#include "BufferPoolManager.hpp"

template <size_t N>
BufferPoolManager<N>::BufferPoolManager(
	PDiskManager _disk_manager, PReplacer<N> _replacer)
	: disk_manager{_disk_manager}, replacer{_replacer} {}

template <size_t N>
auto BufferPoolManager<N>::fetch_page(page_id_t page_id) -> std::optional<CRPage>
{
	// If the page is already in the buffer pool, return it
	if (auto it = page_table.find(page_id); it != page_table.end())
	{
		auto frame_id = it->second;
		auto &frame = frames[frame_id];
		frame.is_pinned = true;
		return frame.page;
	}

	// If the buffer pool is full, evict a page
	if (free_list.size == 0)
	{
		auto frame_id = replacer->Victim();
		if (!frame_id.has_value())
		{
			return std::nullopt;
		}
		auto &frame = frames[frame_id.value()];
		if (frame.is_pinned)
		{
			return std::nullopt;
		}

		page_table.erase(frame.page.id);
		free_list.push(frame_id.value());
	}

	// Fetch the page from the disk
	auto page = disk_manager->read_page(page_id);
	if (!page.has_value())
	{
		return std::nullopt;
	}

	auto frame_id = free_list.pop();
	auto &frame = frames[frame_id];
	frame.page = page.value();
	frame.is_pinned = true;
	page_table[page_id] = frame_id;

	return frame.page;
}
