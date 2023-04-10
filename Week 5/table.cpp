#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Tables.hpp"

using Table = std::vector<std::vector<std::string>>;

Table readCSV(
	const std::string &filename,
	char delim = ',')
{
	std::ifstream file{filename};

	std::istringstream sstr{};
	Table data{};

	std::string line{}, word{};

	std::getline(file, line);
	while (std::getline(file, line))
	{
		std::vector<std::string> vec{};
		sstr.str(line);

		while (std::getline(sstr, word, delim))
		{
			word.erase(0, word.find_first_not_of(" \t\n\r\""));
			word.erase(word.find_last_not_of(" \t\n\r\"") + 1);

			vec.emplace_back(word);
		}

		data.emplace_back(vec);

		sstr.clear();
	}

	file.close();

	return data;
}

int main()
{
	std::cout << "Size of Employee: " << sizeof(Employee) << '\n';
	std::cout << "Size of Company:  " << sizeof(Company) << '\n';

	try
	{
		auto data = readCSV("./files/employee.csv", ',');
		std::vector<Employee> employees{};

		for (auto &&row : data)
		{
			Employee employee{};
			employee.id = std::stoi(row[0]);
			std::strcpy(employee.fname.data(), row[1].c_str());
			std::strcpy(employee.lname.data(), row[2].c_str());
			employee.company_id = std::stoi(row[3]);

			employees.emplace_back(employee);
		}

		std::sort(std::begin(employees), std::end(employees));

		std::ofstream file{"./files/employee.bin", std::ios::binary};

		for (const auto &employee : employees)
		{
			file.write(reinterpret_cast<const char *>(&employee), sizeof(Employee));
		}

		file.close();
		file.clear();

		// Read employees.csv

		data = readCSV("./files/company.csv", ';');

		std::vector<Company> companys{};

		for (const auto &row : data)
		{
			Company company{};
			company.id = std::stoi(row[0]);
			std::strcpy(company.name.data(), row[1].c_str());
			std::strcpy(company.slogan.data(), row[2].c_str());

			companys.emplace_back(company);
		}

		std::sort(std::begin(companys), std::end(companys));

		file.open("./files/company.bin", std::ios::binary);

		for (const auto &company : companys)
		{
			file.write(reinterpret_cast<const char *>(&company), sizeof(Company));
		}

		file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
