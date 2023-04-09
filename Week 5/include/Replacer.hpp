#pragma once

#ifndef _REPLACER_HPP_
#define _REPLACER_HPP_

#include <array>
#include <cstdint>
#include <optional>

#include "Frame.hpp"
#include "Types.hpp"

template <size_t N>
struct Replacer
{
	std::array<Frame, N> *frames{nullptr}; // pointer to the frames

	Replacer() = default;
	virtual ~Replacer() = default;

	/**
	 * @brief Remove the victim frame as defined by the replacement policy.
	 * @return the id of the victim frame, std::nullopt if no victim was found
	 */
	virtual auto victim() -> std::optional<frame_id_t> = 0;

	/**
	 * @brief Pins a frame, indicating that it should not be victimized until it is unpinned.
	 * @param frame_id the id of the frame to pin
	 */
	virtual auto pin(frame_id_t frame_id) -> void = 0;

	/**
	 * @brief Unpins a frame, indicating that it can now be victimized.
	 * @param frame_id the id of the frame to unpin
	 */
	virtual auto unpin(frame_id_t frame_id) -> void = 0;
};

template <size_t N>
using PReplacer = Replacer<N> *;
template <size_t N>
using CPReplacer = const Replacer<N> *;
template <size_t N>
using RReplacer = Replacer<N> &;
template <size_t N>
using CRReplacer = const Replacer<N> &;

#include "Replacer.hpp"

#endif // _REPLACER_HPP_
