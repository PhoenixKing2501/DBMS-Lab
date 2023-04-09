#include <chrono>
#include <format>
#include <iostream>
#include <thread>

using namespace std::literals;

#include "BufferPoolManager.hpp"
#include "ClockReplacer.hpp"
#include "DiskManager.hpp"

#include "Tables.hpp"

constexpr size_t BUFFERSIZE = 3;

int main()
{
	DiskManager disk_manager{};
	ClockReplacer<BUFFERSIZE> replacer{};
	BufferPoolManager<BUFFERSIZE> buffer_manager{&disk_manager, &replacer};

	auto book_pages = disk_manager.add_page("./files/books.bin");
	auto author_pages = disk_manager.add_page("./files/authors.bin");

	if (not book_pages.has_value() or
		not author_pages.has_value())
	{
		std::cerr << "Error opening file" << '\n';
		return EXIT_FAILURE;
	}

	for (auto page_id : *book_pages)
	{
	fetch_book:
		auto page = buffer_manager.fetch_page(page_id);

		if (not page.has_value())
		{
			std::cerr << "No Frame Free" << '\n';
			std::this_thread::sleep_for(100ms);
			goto fetch_book;
		}

		auto books = *reinterpret_cast<
			std::array<Books, page->data.size() / sizeof(Books)> *>(&page->data);

		for (auto &book : books)
		{
			if (book == Books{})
				break;

			std::this_thread::sleep_for(50ms);

			std::cout << std::format("{:3}\t{:3}\t{:20}\t{:20}\n",
									 book.id,
									 book.author_id,
									 book.title.data(),
									 book.type.data());
		}

		std::thread t{[&buffer_manager, page_id]()
					  {
						  std::this_thread::sleep_for(2s);
						  buffer_manager.unpin_page(page_id);
						  std::cerr << "Unpinning page " << page_id << std::endl;
					  }};
		t.detach();
	}

	for (auto page_id : *author_pages)
	{
	fetch_author:
		auto page = buffer_manager.fetch_page(page_id);

		if (not page.has_value())
		{
			std::cerr << "No Frame Free" << '\n';
			std::this_thread::sleep_for(100ms);
			goto fetch_author;
		}

		auto authors = *reinterpret_cast<
			std::array<Authors, page->data.size() / sizeof(Authors)> *>(page->data.data());
		for (auto &&author : authors)
		{
			if (author == Authors{})
				break;

			std::this_thread::sleep_for(50ms);

			std::cout << std::format("{:3}\t{:20}\t{:20}\n",
									 author.id,
									 author.fname.data(),
									 author.lname.data());
		}

		std::thread t{[&buffer_manager, page_id]()
					  {
						  std::this_thread::sleep_for(2s);
						  buffer_manager.unpin_page(page_id);
						  std::cerr << "Unpinning page " << page_id << std::endl;
					  }};
		t.detach();
	}

	std::cout << "Number of I/Os: " << disk_manager.num_ios << '\n'
			  << "Number of pages: " << disk_manager.pages.size() << '\n';
}
