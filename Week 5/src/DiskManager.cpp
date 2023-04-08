#include "DiskManager.hpp"

auto DiskManager::read(page_id_t page_id) -> std::optional<std::array<char, PAGE_SIZE>>
{
	++num_ios;
	if (pages.find(page_id) != std::end(pages))
		return pages[page_id].data;

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
		Page page{};
		page.data = std::move(data);
		pages[page.id] = std::move(page);
	}
}
