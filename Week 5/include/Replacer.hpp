#pragma once

#ifndef _REPLACER_HPP_
#define _REPLACER_HPP_

#include <cstdint>
#include <optional>

struct Replacer
{
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
	virtual auto pin(int32_t frame_id) -> void = 0;

	/**
	 * @brief Unpins a frame, indicating that it can now be victimized.
	 * @param frame_id the id of the frame to unpin
	 */
	virtual auto unpin(int32_t frame_id) -> void = 0;

	/**
	 * @return the number of elements in the replacer that can be victimized
	 */
	virtual size_t size() = 0;
};

using PReplacer = Replacer *;
using CPReplacer = const Replacer *;
using RReplacer = Replacer &;
using CRReplacer = const Replacer &;

#endif // _REPLACER_HPP_
