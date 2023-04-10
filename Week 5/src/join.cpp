/**
 * Simulation the following join operation:
 * SELECT
 * 		employee.first_name, employee.last_name, company.fname
 * FROM
 * 		employee
 * INNER JOIN
 * 		company
 * ON
 * 		employee.company_id = company.id
 */

#include <chrono>
#include <format>
#include <iostream>
#include <random>
#include <thread>

using namespace std::literals;

#include "BufferPoolManager.hpp"
#include "ClockReplacer.hpp"
#include "DiskManager.hpp"
#include "LRUReplacer.hpp"
#include "MRUReplacer.hpp"
#include "Types.hpp"

#include "Tables.hpp"

constexpr size_t BUFFERSIZE = 32;

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

	auto _company_pages = disk_manager.add_page("./files/company.bin");
	auto _employee_pages = disk_manager.add_page("./files/employee.bin");

	auto fetch_page = [&](page_id_t page_id) -> Page
	{
		static std::mt19937_64 gen{std::random_device{}()};
		static std::uniform_int_distribution dis{10, 20};

		while (true)
		{
			auto page = buffer_pool_manager.fetch_page(page_id);
			if (page.has_value())
				return *page;

			std::fputs("No Frame Free\n", stderr);
			std::this_thread::sleep_for(std::chrono::milliseconds{dis(gen)});
		}
	};

	auto unpin_page = [&](page_id_t page_id)
	{
		static std::mt19937_64 gen{std::random_device{}()};
		static std::uniform_int_distribution dis{10, 20};

		std::this_thread::sleep_for(std::chrono::milliseconds{dis(gen)});
		buffer_pool_manager.unpin_page(page_id);
	};

	if (not _company_pages.has_value() or
		not _employee_pages.has_value())
	{
		fputs("Error opening file", stderr);
		return EXIT_FAILURE;
	}

	auto company_pages = _company_pages->begin();
	auto employee_pages = _employee_pages->begin();

	std::cout << std::format("{:30}\t{:30}\t{:50}\n\n",
							 "First Name", "Last Name", "Company Name");

	// Merge-Join Algorithm

	// Fetch the first page in the employees table
	auto employee_page = fetch_page(*employee_pages);

	// Fetch the first page in the companys table
	auto company_page = fetch_page(*company_pages);

	// Get the first tuple of the employees table
	auto employee_tuple = page_to_array<Employee>(employee_page)->begin();

	// Get the first tuple of the companys table
	auto company_tuple = page_to_array<Company>(company_page)->begin();

	while (employee_pages != _employee_pages->end() or
		   company_pages != _company_pages->end())
	{
		if (*company_tuple == Company{} or
			*employee_tuple == Employee{})
			break;
			
		if (company_tuple->id < employee_tuple->company_id)
		{
			++company_tuple;

			if (company_tuple == page_to_array<Company>(company_page)->end())
			{
				(std::thread{unpin_page, *company_pages}).detach();
				// buffer_pool_manager.unpin_page(*company_pages);
				++company_pages;

				if (company_pages == _company_pages->end())
					break;

				company_page = fetch_page(*company_pages);
				company_tuple = page_to_array<Company>(company_page)->begin();
			}
		}
		else if (company_tuple->id > employee_tuple->company_id)
		{
			++employee_tuple;

			if (employee_tuple == page_to_array<Employee>(employee_page)->end())
			{
				(std::thread{unpin_page, *employee_pages}).detach();
				// buffer_pool_manager.unpin_page(*employee_pages);
				++employee_pages;

				if (employee_pages == _employee_pages->end())
					break;

				employee_page = fetch_page(*employee_pages);
				employee_tuple = page_to_array<Employee>(employee_page)->begin();
			}
		}

		else // company_tuple->id == employee_tuple->company_id
		{
			std::cout << std::format("{:30}\t{:30}\t{:50}\n",
									 employee_tuple->fname.data(),
									 employee_tuple->lname.data(),
									 company_tuple->name.data());

			++employee_tuple;

			if (employee_tuple == page_to_array<Employee>(employee_page)->end())
			{
				(std::thread{unpin_page, *employee_pages}).detach();
				// buffer_pool_manager.unpin_page(*employee_pages);
				++employee_pages;

				if (employee_pages == _employee_pages->end())
					break;

				employee_page = fetch_page(*employee_pages);
				employee_tuple = page_to_array<Employee>(employee_page)->begin();
			}
		}
	}

	std::cout << "\nNumber of I/Os: " << disk_manager.num_ios << '\n'
			  << "Number of pages: " << disk_manager.pages.size() << '\n';
}
