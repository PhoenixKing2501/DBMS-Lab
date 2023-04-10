#include "DiskManager.hpp"

auto DiskManager::read_page(page_id_t page_id)
	-> std::optional<Page>
{
	++num_ios;

	if (pages.find(page_id) != std::end(pages))
	{
		std::ifstream file{pages[page_id].filename, std::ios::binary};
		if (not file.is_open())
			return std::nullopt;

		file.seekg(pages[page_id].offset);
		std::array<char, PAGE_SIZE> data{};
		file.read(reinterpret_cast<char *>(data.data()), PAGE_SIZE);

		if (file.gcount() == 0)
			return std::nullopt;
		
		return Page{page_id, data};
	}

	return std::nullopt;
}

auto DiskManager::add_page(const std::string &filename)
	-> std::optional<std::vector<page_id_t>>
{
	std::ifstream file{filename, std::ios::binary};
	if (not file.is_open())
		return std::nullopt;

	std::vector<page_id_t> page_ids{};

	file.seekg(0, std::ios::end);
	const auto file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	for (size_t i = 0; i < file_size; i += PAGE_SIZE)
	{
		const auto page_id = Page::generate_page_id();
		pages.emplace(page_id, PageInfo{filename, i});
		page_ids.push_back(page_id);
	}

	return page_ids;
}
