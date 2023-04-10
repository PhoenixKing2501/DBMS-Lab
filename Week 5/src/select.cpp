/**
 * Simulating the following SQL statement:
 * SELECT
 * 		employee.first_name, employee.last_name, company.name
 * FROM
 * 		employee, company
 * WHERE
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

	auto company_pages = *_company_pages;
	auto employee_pages = *_employee_pages;

	std::cout << std::format("{:30}\t{:30}\t{:50}\n\n",
							 "First Name", "Last Name", "Company Name");

	for (auto employee_page_id : employee_pages)
	{
		auto page = fetch_page(employee_page_id);

		auto employees = *page_to_array<Employee>(page);

		for (auto &employee : employees)
		{
			if (employee == Employee{})
				break;

			for (auto company_page_id : company_pages)
			{
				auto page = fetch_page(company_page_id);

				auto companys = *page_to_array<Company>(page);

				for (auto &company : companys)
				{
					if (company == Company{})
						break;

					// Select Condition
					if (company.id == employee.company_id)
					{
						std::cout << std::format("{:30}\t{:30}\t{:50}\n",
												 employee.fname.data(),
												 employee.lname.data(),
												 company.name.data());
					}
				}

				(std::thread{unpin_page, company_page_id}).detach();
				// buffer_pool_manager.unpin_page(company_page_id);
			}
		}

		(std::thread{unpin_page, employee_page_id}).detach();
		// buffer_pool_manager.unpin_page(employee_page_id);
	}

	std::cout << "\nNumber of I/Os: " << disk_manager.num_ios << '\n'
			  << "Number of pages: " << disk_manager.pages.size() << '\n';
}
