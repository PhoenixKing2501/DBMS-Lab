#include <algorithm>

template <size_t N>
auto ClockReplacer<N>::victim() -> std::optional<frame_id_t>
{
	if (not this->frames)
	{
		return std::nullopt;
	}

	auto victim = std::find_if(
		this->frames->begin(),
		this->frames->end(),
		[](CRFrame frame)
		{
			// return not frame.is_pinned;
			return frame.pin_count == 0;
		});

	if (victim == this->frames->end())
	{
		return std::nullopt;
	}

	while (true)
	{
		if (this->reference_bits[this->clock_hand])
		{
			this->reference_bits[this->clock_hand] = false;
		}
		else
		{
			// if (not(*this->frames)[this->clock_hand].is_pinned)
			if ((*this->frames)[this->clock_hand].pin_count == 0)
			{
				return static_cast<frame_id_t>(this->clock_hand);
			}
		}

		this->clock_hand = (this->clock_hand + 1) % this->frames->size();
	}
}

template <size_t N>
auto ClockReplacer<N>::pin(frame_id_t frame_id) -> void
{
	if (this->frames and frame_id < this->frames->size())
	{
		// (*this->frames)[frame_id].is_pinned = true;
		this->reference_bits[frame_id] = true;
		(*this->frames)[frame_id].pin_count++;
	}
}

template <size_t N>
auto ClockReplacer<N>::unpin(frame_id_t frame_id) -> void
{
	if (this->frames and frame_id < this->frames->size())
	{
		// (*this->frames)[frame_id].is_pinned = false;
		(*this->frames)[frame_id].pin_count--;
	}
}
