#pragma once

#ifndef _CLOCK_REPLACER_HPP_
#define _CLOCK_REPLACER_HPP_

#include "Replacer.hpp"

template <size_t N>
struct ClockReplacer : public Replacer<N>
{
	size_t clock_hand{0};
	std::array<bool, N> reference_bits{false};

	ClockReplacer() = default;
	virtual ~ClockReplacer() = default;

	/**
	 * @brief Remove the victim frame as defined by the replacement policy.
	 * @return the id of the victim frame, std::nullopt if no victim was found
	 */
	auto victim() -> std::optional<frame_id_t> override;

	/**
	 * @brief Pins a frame, indicating that it should not be victimized until it is unpinned.
	 * @param frame_id the id of the frame to pin
	 */
	auto pin(frame_id_t frame_id) -> void override;

	/**
	 * @brief Unpins a frame, indicating that it can now be victimized.
	 * @param frame_id the id of the frame to unpin
	 */
	auto unpin(frame_id_t frame_id) -> void override;
};

#include "ClockReplacer.inl"

#endif // _CLOCK_REPLACER_HPP_
