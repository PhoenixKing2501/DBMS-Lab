#pragma once

#ifndef _LRU_REPLACER_HPP_
#define _LRU_REPLACER_HPP_

#include <list>
#include <optional>
#include <unordered_map>

#include "Replacer.hpp"

template <size_t N>
struct LRUReplacer : public Replacer<N>
{
	std::list<frame_id_t> lru_list{};
	std::unordered_map<frame_id_t, typename std::list<frame_id_t>::iterator> lru_map{};

	LRUReplacer() = default;	
	virtual ~LRUReplacer() = default;

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

#include "LRUReplacer.inl"

#endif // _LRU_REPLACER_HPP_
