#pragma once

#ifndef _MRU_REPLACER_HPP_
#define _MRU_REPLACER_HPP_

#include <forward_list>
#include <optional>
#include <unordered_map>

#include "Replacer.hpp"

template <size_t N>
struct MRUReplacer : public Replacer<N>
{
	std::forward_list<frame_id_t> mru_list{};
	std::unordered_map<frame_id_t, typename std::forward_list<frame_id_t>::iterator> mru_map{};

	MRUReplacer() = default;
	virtual ~MRUReplacer() = default;

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

#include "MRUReplacer.inl"

#endif // _MRU_REPLACER_HPP_
