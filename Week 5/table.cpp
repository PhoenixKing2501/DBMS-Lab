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
			word.erase(0, word.find_first_not_of(" \t\n\r"));
			word.erase(word.find_last_not_of(" \t\n\r") + 1);

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
	std::cout << "Size of Books: " << sizeof(Books) << '\n';
	std::cout << "Size of Authors: " << sizeof(Authors) << '\n';

	try
	{
		auto data = readCSV("./files/books.csv");
		std::vector<Books> books{};

		for (auto &&row : data)
		{
			Books book{};
			book.id = std::stoi(row[0]);
			std::strcpy(book.title, row[1].c_str());
			std::strcpy(book.type, row[2].c_str());
			book.author_id = std::stoi(row[3]);

			books.emplace_back(book);
		}

		// add empty book to books as sentinel
		books.emplace_back(Books{});

		std::ofstream file{"./files/books.bin", std::ios::binary};

		for (const auto &book : books)
		{
			file.write(reinterpret_cast<const char *>(&book), sizeof(Books));
		}

		file.close();

		// Read books.csv

		data = readCSV("./files/authors.csv");

		std::vector<Authors> authors{};

		for (const auto &row : data)
		{
			Authors author{};
			author.id = std::stoi(row[0]);
			std::strcpy(author.fname, row[1].c_str());
			std::strcpy(author.lname, row[2].c_str());

			authors.emplace_back(author);
		}

		// add empty author to authors as sentinel
		authors.emplace_back(Authors{});

		file.open("./files/authors.bin", std::ios::binary);

		for (const auto &author : authors)
		{
			file.write(reinterpret_cast<const char *>(&author), sizeof(Authors));
		}

		file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
