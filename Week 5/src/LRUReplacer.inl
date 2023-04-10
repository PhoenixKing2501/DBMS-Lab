template <size_t N>
auto LRUReplacer<N>::victim() -> std::optional<frame_id_t>
{
	if (not this->frames)
	{
		return std::nullopt;
	}

	for (auto it = this->lru_list.rbegin(); it != this->lru_list.rend(); ++it)
	{
		if ((*this->frames)[*it].pin_count == 0)
		{
			auto victim = *it;
			this->lru_list.erase(this->lru_map[victim]);
			this->lru_map.erase(victim);
			return victim;
		}
	}

	return std::nullopt;
}

template <size_t N>
auto LRUReplacer<N>::pin(frame_id_t frame_id) -> void
{
	if (this->lru_map.find(frame_id) != this->lru_map.end())
	{
		this->lru_list.erase(this->lru_map[frame_id]);
	}
	this->lru_list.push_front(frame_id);
	this->lru_map[frame_id] = this->lru_list.begin();
	++(*this->frames)[frame_id].pin_count;
}

template <size_t N>
auto LRUReplacer<N>::unpin(frame_id_t frame_id) -> void
{
	if (this->lru_map.find(frame_id) != this->lru_map.end())
	{
		--(*this->frames)[frame_id].pin_count;
	}
}
