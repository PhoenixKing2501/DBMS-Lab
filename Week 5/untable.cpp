#include <format>
#include <fstream>
#include <iostream>

#include "Tables.hpp"

int main()
{
	try
	{
		std::ifstream file{"./files/books.bin", std::ios::binary};

		if (!file.is_open())
		{
			std::cerr << "Error opening file" << '\n';
			return 1;
		}

		Books book{};

		while (file.read(reinterpret_cast<char *>(&book), sizeof(Books)))
		{
			std::cout << std::format("{:3}\t{:3}\t{:20}\t{:20}\n",
									 book.id,
									 book.author_id,
									 book.title.data(),
									 book.type.data());
		}

		file.close();
		file.clear();

		file.open("./files/authors.bin", std::ios::binary);

		if (!file.is_open())
		{
			std::cerr << "Error opening file" << '\n';
			return 1;
		}

		Authors author{};

		while (file.read(reinterpret_cast<char *>(&author), sizeof(Authors)))
		{
			std::cout << std::format("{:3}\t{:20}\t{:20}\n",
									 author.id,
									 author.fname.data(),
									 author.lname.data());
		}

		file.close();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}
