/**
 * Simulating the following SQL statement:
 * SELECT
 * 		authors.first_name, authors.last_name, books.title
 * FROM
 * 		authors, books
 * WHERE
 * 		authors.id = books.author_id
 * AND
 * 		books.type = 'original';
 */

#include <chrono>
#include <format>
#include <iostream>
#include <thread>

using namespace std::literals;

#include "BufferPoolManager.hpp"
// #include "ClockReplacer.hpp"
// #include "LRUReplacer.hpp"
#include "MRUReplacer.hpp"
#include "DiskManager.hpp"
#include "Types.hpp"

#include "Tables.hpp"

constexpr size_t BUFFERSIZE = 4;

int main()
{
	DiskManager disk_manager{};
	// auto replacer = ClockReplacer<BUFFERSIZE>{};
	// auto replacer = LRUReplacer<BUFFERSIZE>{};
	auto replacer = MRUReplacer<BUFFERSIZE>{};
	BufferPoolManager<BUFFERSIZE> buffer_pool_manager{&disk_manager, &replacer};

	auto _book_pages = disk_manager.add_page("./files/books.bin");
	auto _author_pages = disk_manager.add_page("./files/authors.bin");

	auto fetch_page = [&](page_id_t page_id) -> Page
	{
		while (true)
		{
			auto page = buffer_pool_manager.fetch_page(page_id);
			if (page.has_value())
				return *page;
			std::fputs("No Frame Free\n", stderr);
			std::this_thread::sleep_for(100ms);
		}
	};

	auto unpin_page = [&](page_id_t page_id)
	{
		std::this_thread::sleep_for(500ms);
		buffer_pool_manager.unpin_page(page_id);
	};

	if (not _book_pages.has_value() or
		not _author_pages.has_value())
	{
		fputs("Error opening file", stderr);
		return EXIT_FAILURE;
	}

	auto book_pages = *_book_pages;
	auto author_pages = *_author_pages;

	std::cout << std::format("{:20}\t{:20}\t{:20}\n",
							 "First Name", "Last Name", "Title");

	for (auto book_page_id : book_pages)
	{
		auto page = fetch_page(book_page_id);

		auto books = *reinterpret_cast<
			std::array<Books, PAGE_SIZE / sizeof(Books)> *>(&page.data);

		for (auto &book : books)
		{
			if (book == Books{})
				break;

			for (auto author_page_id : author_pages)
			{
				auto page = fetch_page(author_page_id);

				auto authors = *reinterpret_cast<
					std::array<Authors, PAGE_SIZE / sizeof(Authors)> *>(&page.data);

				for (auto &author : authors)
				{
					if (author == Authors{})
						break;

					// Select Condition
					if (author.id == book.author_id and					// authors.id = books.author_id
						std::strcmp(book.type.data(), "original") == 0) // books.type = 'original'
					{
						std::cout << std::format("{:20}\t{:20}\t{:20}\n",
												 author.fname.data(),
												 author.lname.data(),
												 book.title.data());
					}
				}

				(std::thread{unpin_page, author_page_id}).detach();
				// buffer_pool_manager.unpin_page(author_page_id);
			}
		}

		(std::thread{unpin_page, book_page_id}).detach();
		// buffer_pool_manager.unpin_page(book_page_id);
	}

	std::cout << "\nNumber of I/Os: " << disk_manager.num_ios << '\n'
			  << "Number of pages: " << disk_manager.pages.size() << '\n';
}
