#include "ClockReplacer.hpp"

#include <algorithm>

template <size_t N>
auto ClockReplacer<N>::victim() -> std::optional<frame_id_t>
{
	if (not this->frames)
	{
		return std::nullopt;
	}

	auto victim = std::find_if(
		std::cbegin(this->frames), std::cend(this->frames),
		[](CRFrame frame)
		{ return not frame.is_pinned; });

	if (victim == std::end(this->frames))
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
			if (not(*this->frames)[this->clock_hand].is_pinned)
			{
				return static_cast<frame_id_t>(this->clock_hand);
			}
		}

		this->clock_hand = (this->clock_hand + 1) % std::size(this->frames);
	}
}

template <size_t N>
auto ClockReplacer<N>::pin(frame_id_t frame_id) -> void
{
	if (this->frames and frame_id < std::size(this->frames))
	{
		(*this->frames)[frame_id].is_pinned = true;
	}
}

template <size_t N>
auto ClockReplacer<N>::unpin(frame_id_t frame_id) -> void
{
	if (this->frames and frame_id < std::size(this->frames))
	{
		(*this->frames)[frame_id].is_pinned = false;
	}
}

template <size_t N>
auto ClockReplacer<N>::size() -> size_t
{
	if (not this->frames)
	{
		return 0;
	}

	return std::count_if(
		std::cbegin(this->frames), std::cend(this->frames),
		[](CRFrame frame)
		{ return not frame.is_pinned; });
}
