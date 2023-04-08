#in

using Table = std::vector<std::vector<std::string>>;

Table readCSV(
	const std::string &filename,
	char delim = ',')
{
	std::ifstream file{filename};
	std::istringstream sstr{};
	Table data{};

	std::string line{}, word{};

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
