/**
 * Simulation the following join operation:
 * SELECT
 * 		authors.first_name, authors.last_name, books.title
 * FROM
 * 		authors
 * INNER JOIN
 * 		books
 * ON
 * 		authors.id = books.author_id
 * WHERE
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

template <typename T>
auto page_to_array(RPage page) -> std::array<T, PAGE_SIZE / sizeof(T)> *
{
	return reinterpret_cast<std::array<T, PAGE_SIZE / sizeof(T)> *>(&page.data);
};

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

	auto book_pages = _book_pages->begin();
	auto author_pages = _author_pages->begin();

	std::cout << std::format("{:20}\t{:20}\t{:20}\n",
							 "First Name", "Last Name", "Title");

	// Merge-Join Algorithm

	// Fetch the first page in the authors table
	auto author_page = fetch_page(*author_pages);

	// Fetch the first page in the books table
	auto book_page = fetch_page(*book_pages);

	// Get the first tuple of the authors table
	auto author_tuple = page_to_array<Authors>(author_page)->begin();

	// Get the first tuple of the books table
	auto book_tuple = page_to_array<Books>(book_page)->begin();

	while (author_pages != _author_pages->end() or
		   book_pages != _book_pages->end())
	{
		if (author_tuple->id < book_tuple->author_id)
		{
			++author_tuple;

			if (author_tuple == page_to_array<Authors>(author_page)->end())
			{
				(std::thread{unpin_page, *author_pages}).detach();
				++author_pages;

				if (author_pages == _author_pages->end())
					break;

				author_page = fetch_page(*author_pages);
				author_tuple = page_to_array<Authors>(author_page)->begin();
			}
		}
		else if (author_tuple->id > book_tuple->author_id)
		{
			++book_tuple;

			if (book_tuple == page_to_array<Books>(book_page)->end())
			{
				(std::thread{unpin_page, *book_pages}).detach();
				++book_pages;

				if (book_pages == _book_pages->end())
					break;

				book_page = fetch_page(*book_pages);
				book_tuple = page_to_array<Books>(book_page)->begin();
			}
		}

		else // author_tuple->id == book_tuple->author_id
		{
			if (std::strcmp(book_tuple->type.data(), "original") == 0) // books.type = 'original'
			{
				std::cout << std::format("{:20}\t{:20}\t{:20}\n",
										 author_tuple->fname.data(),
										 author_tuple->lname.data(),
										 book_tuple->title.data());
			}

			++book_tuple;

			if (book_tuple == page_to_array<Books>(book_page)->end())
			{
				(std::thread{unpin_page, *book_pages}).detach();
				++book_pages;

				if (book_pages == _book_pages->end())
					break;

				book_page = fetch_page(*book_pages);
				book_tuple = page_to_array<Books>(book_page)->begin();
			}
		}
	}

	std::cout << "\nNumber of I/Os: " << disk_manager.num_ios << '\n'
			  << "Number of pages: " << disk_manager.pages.size() << '\n';
}
