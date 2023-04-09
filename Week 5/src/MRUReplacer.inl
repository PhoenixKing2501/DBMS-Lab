template <size_t N>
auto MRUReplacer<N>::victim() -> std::optional<frame_id_t>
{
	if (not this->frames)
	{
		return std::nullopt;
	}

	for (auto it = this->mru_list.begin(); it != this->mru_list.end(); ++it)
	{
		if ((*this->frames)[*it].pin_count == 0)
		{
			auto victim = *it;
			this->mru_list.erase_after(this->mru_map[victim]);
			this->mru_map.erase(victim);
			return victim;
		}
	}

	return std::nullopt;
}

template <size_t N>
auto MRUReplacer<N>::pin(frame_id_t frame_id) -> void
{
	if (this->mru_map.find(frame_id) != this->mru_map.end())
	{
		this->mru_list.erase_after(this->mru_map[frame_id]);
	}
	this->mru_list.push_front(frame_id);
	this->mru_map[frame_id] = this->mru_list.before_begin();
	++(*this->frames)[frame_id].pin_count;
}

template <size_t N>
auto MRUReplacer<N>::unpin(frame_id_t frame_id) -> void
{
	if (this->mru_map.find(frame_id) != this->mru_map.end())
	{
		--(*this->frames)[frame_id].pin_count;
	}
}
