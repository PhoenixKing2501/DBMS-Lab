template <size_t N>
BufferPoolManager<N>::BufferPoolManager(
	PDiskManager _disk_manager, PReplacer<N> _replacer)
	: disk_manager{_disk_manager}, replacer{_replacer}
{
	replacer->frames = &this->frames;
}

template <size_t N>
auto BufferPoolManager<N>::fetch_page(page_id_t page_id) -> std::optional<Page>
{
	// If the page is already in the buffer pool, return it
	if (auto it = page_table.find(page_id); it != std::end(page_table))
	{
		auto frame_id = it->second;
		auto &frame = frames[frame_id];
		replacer->pin(frame_id);
		std::fprintf(stderr, "Hit page %u\n", page_id);
		std::fprintf(stderr, "Pinning Page %u Pin Count %hu\n", page_id, frame.pin_count);
		return frame.page;
	}

	// If the buffer pool is full, evict a page
	if (free_list.size == 0)
	{
		auto frame_id = replacer->victim();
		if (!frame_id.has_value())
		{
			return std::nullopt;
		}
		auto &frame = frames[frame_id.value()];
		// if (frame.is_pinned)
		if (frame.pin_count > 0)
		{
			return std::nullopt;
		}

		page_table.erase(frame.page.id);
		std::fprintf(stderr, "Evicting page %d\n", frame.page.id);

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
	replacer->pin(frame_id);
	std::fprintf(stderr, "Fetching page %u\n", page_id);
	std::fprintf(stderr, "Pinning Page %u Pin Count %hu\n", page_id, frame.pin_count);
	page_table[page_id] = frame_id;

	return frame.page;
}

template <size_t N>
auto BufferPoolManager<N>::unpin_page(page_id_t page_id) -> bool
{
	if (auto it = page_table.find(page_id); it != std::end(page_table))
	{
		auto frame_id = it->second;
		auto &frame = frames[frame_id];
		replacer->unpin(frame_id);
		std::fprintf(stderr, "Unpinning page %u Pin Count %hu\n", page_id, frame.pin_count);
		return true;
	}
	return false;
}
