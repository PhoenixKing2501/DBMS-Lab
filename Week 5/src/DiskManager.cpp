#include "DiskManager.hpp"

auto DiskManager::read_page(page_id_t page_id)
	-> std::optional<Page>
{
	++num_ios;
	if (pages.find(page_id) != std::end(pages))
		return pages[page_id];

	return std::nullopt;
}

auto DiskManager::add_page(const std::string &filename)
	-> std::optional<std::vector<page_id_t>>
{
	std::ifstream file{filename, std::ios::binary};
	if (!file.is_open())
		return std::nullopt;

	std::array<char, PAGE_SIZE> data{};
	std::vector<page_id_t> page_ids{};
	while (file.read(data.data(), PAGE_SIZE))
	{
		Page page{Page::generate_page_id(), std::move(data)};
		pages[page.id] = std::move(page);
		page_ids.push_back(page.id);

		data.fill(0);
	}

	if (file.gcount() > 0)
	{
		Page page{Page::generate_page_id(), std::move(data)};
		pages[page.id] = std::move(page);
		page_ids.push_back(page.id);
	}

	return page_ids;
}
