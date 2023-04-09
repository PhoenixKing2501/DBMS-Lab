#include <chrono>
#include <format>
#include <iostream>
#include <thread>

using namespace std::literals;

#include "BufferPoolManager.hpp"
#include "ClockReplacer.hpp"
#include "DiskManager.hpp"

#include "Tables.hpp"

constexpr size_t BUFFERSIZE = 256;

int main()
{
	DiskManager disk_manager{};
	ClockReplacer<BUFFERSIZE> replacer{};
	BufferPoolManager<BUFFERSIZE> buffer_pool_manager{&disk_manager, &replacer};

	auto employee_pages = disk_manager.add_page("./files/employee.bin");
	auto company_pages = disk_manager.add_page("./files/company.bin");

	if (not employee_pages.has_value() or
		not company_pages.has_value())
	{
		std::cerr << "Error opening file" << '\n';
		return EXIT_FAILURE;
	}

	for (auto page_id : *employee_pages)
	{
	fetch_employee:
		auto page = buffer_pool_manager.fetch_page(page_id);

		if (not page.has_value())
		{
			std::cerr << "No Frame Free" << '\n';
			// std::this_thread::sleep_for(100ms);
			goto fetch_employee;
		}

		auto employees = *reinterpret_cast<
			std::array<Employee, PAGE_SIZE / sizeof(Employee)> *>(&page->data);

		for (auto &employee : employees)
		{
			if (employee == Employee{})
				break;

			// std::this_thread::sleep_for(50ms);

			std::cout << std::format("{:3}\t{:3}\t{:20}\t{:20}\n",
									 employee.id,
									 employee.company_id,
									 employee.fname.data(),
									 employee.lname.data());
		}

		std::thread t{[&buffer_pool_manager, page_id]()
					  {
						//   std::this_thread::sleep_for(2s);
						  buffer_pool_manager.unpin_page(page_id);
						//   std::cerr << "Unpinning page " << page_id << std::endl;
					  }};
		t.detach();
	}

	for (auto page_id : *company_pages)
	{
	fetch_company:
		auto page = buffer_pool_manager.fetch_page(page_id);

		if (not page.has_value())
		{
			std::cerr << "No Frame Free" << '\n';
			// std::this_thread::sleep_for(100ms);
			goto fetch_company;
		}

		auto companys = *reinterpret_cast<
			std::array<Company, PAGE_SIZE / sizeof(Company)> *>(page->data.data());
		for (auto &&company : companys)
		{
			if (company == Company{})
				break;

			// std::this_thread::sleep_for(50ms);

			std::cout << std::format("{:3}\t{:20}\t{:20}\n",
									 company.id,
									 company.name.data(),
									 company.slogan.data());
		}

		std::thread t{[&buffer_pool_manager, page_id]()
					  {
						//   std::this_thread::sleep_for(2s);
						  buffer_pool_manager.unpin_page(page_id);
						//   std::cerr << "Unpinning page " << page_id << std::endl;
					  }};
		t.detach();
	}

	std::cout << "Number of I/Os: " << disk_manager.num_ios << '\n'
			  << "Number of pages: " << disk_manager.pages.size() << '\n';
}
