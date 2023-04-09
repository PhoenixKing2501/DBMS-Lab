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
 */

#include <chrono>
#include <format>
#include <iostream>
#include <thread>

using namespace std::literals;

#include "BufferPoolManager.hpp"
#include "ClockReplacer.hpp"
#include "DiskManager.hpp"

#include "Tables.hpp"

constexpr size_t BUFFERSIZE = 4;

int main()
{
	DiskManager disk_manager{};
	ClockReplacer<BUFFERSIZE> replacer{};
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
	++author_pages;

	// Fetch the first page in the books table
	auto book_page = fetch_page(*book_pages);
	++book_pages;

	// Get the first tuple of the authors table
	auto authors_tuple = reinterpret_cast<std::array<Authors, author_page.data.size() / sizeof(Authors)> *>(&author_page.data)->begin();

	// Get the first tuple of the books table
	auto books_tuple = reinterpret_cast<std::array<Books, book_page.data.size() / sizeof(Books)> *>(&book_page.data)->begin();

	// Will do later

	std::cout << "\nNumber of I/Os: " << disk_manager.num_ios << '\n'
			  << "Number of pages: " << disk_manager.pages.size() << '\n';
}
