#include <format>
#include <fstream>
#include <iostream>

#include "Tables.hpp"

int main()
{
	try
	{
		std::ifstream file{"./files/employee.bin", std::ios::binary};

		if (!file.is_open())
		{
			std::cerr << "Error opening file" << '\n';
			return 1;
		}

		Employee employee{};

		while (file.read(reinterpret_cast<char *>(&employee), sizeof(Employee)))
		{
			std::cout << std::format("{:3}\t{:3}\t{:60}\t{:60}\n",
									 employee.id,
									 employee.company_id,
									 employee.fname.data(),
									 employee.lname.data());
		}

		file.close();
		file.clear();

		file.open("./files/company.bin", std::ios::binary);

		if (!file.is_open())
		{
			std::cerr << "Error opening file" << '\n';
			return 1;
		}

		Company company{};

		while (file.read(reinterpret_cast<char *>(&company), sizeof(Company)))
		{
			std::cout << std::format("{:3}\t{:62}\t{:62}\n",
									 company.id,
									 company.name.data(),
									 company.slogan.data());
		}

		file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
