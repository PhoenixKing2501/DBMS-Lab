#include "DiskManager.hpp"

auto DiskManager::read_page(page_id_t page_id)
	-> std::optional<Page>
{
	++num_ios;
	if (pages.find(page_id) != std::end(pages))
		return pages[page_id];
		
	return std::nullopt;
}

auto DiskManager::add_page(const std::string &filename) -> void
{
	std::ifstream file{filename, std::ios::binary};
	if (!file.is_open())
		return;

	std::array<char, PAGE_SIZE> data{};
	while (file.read(data.data(), PAGE_SIZE))
	{
		Page page{Page::generate_page_id(), std::move(data)};
		pages[page.id] = std::move(page);
	}
}
