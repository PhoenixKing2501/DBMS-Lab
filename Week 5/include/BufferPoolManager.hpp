#pragma once

#ifndef _BUFFER_POOL_MANAGER_HPP_
#define _BUFFER_POOL_MANAGER_HPP_

#include <array>
#include <numeric>
#include <optional>
#include <unordered_map>

#include "Types.hpp"

#include "DiskManager.hpp"
#include "Frame.hpp"
#include "Page.hpp"
#include "Replacer.hpp"

template <size_t N = 10>
struct BufferPoolManager
{
	struct
	{
		std::array<frame_id_t, N> free_list{
			[&]() -> std::array<frame_id_t, N>
			{
				std::array<frame_id_t, N> free_list{};
				std::iota(std::rbegin(free_list), std::rend(free_list), 0);
				return free_list;
			}()};

		size_t size{N};

		/**
		 * @brief Pop a frame from the free list
		 * @return The frame id
		 */
		auto pop() -> frame_id_t { return free_list[--size]; }

		/**
		 * @brief Push a frame to the free list
		 * @param frame_id The frame id to push
		 */
		auto push(frame_id_t frame_id) -> void { free_list[size++] = frame_id; }
	} free_list{}; // The free list of frames in the buffer pool

	std::array<Frame, N> frames{};							// The buffer pool
	std::unordered_map<page_id_t, frame_id_t> page_table{}; // page_id -> frame_id
	PDiskManager disk_manager{nullptr};						// Don't delete these two... They should be deleted by the caller
	PReplacer<N> replacer{nullptr};							// Don't delete these two... They should be deleted by the caller

	/**
	 * @brief Creates a new BufferPoolManager
	 * @param disk_manager disk manager instance
	 * @param replacer replacer instance
	 */
	explicit BufferPoolManager(PDiskManager disk_manager, PReplacer<N> replacer);

	/**
	 * @brief Destroys the BufferPoolManager
	 */
	~BufferPoolManager() = default;

	/**
	 * @brief Fetches a page from the buffer pool, and pins it
	 * @param page_id The page id to fetch
	 * @return The page if the page is fetched successfully, std::nullopt otherwise
	 */
	auto fetch_page(page_id_t page_id) -> std::optional<Page>;

	/**
	 * @brief Unpins a page
	 * @param page_id The page id to unpin
	 * @return true if the page is unpinned successfully, false otherwise
	 */
	auto unpin_page(page_id_t page_id) -> bool;
};

#include "BufferPoolManager.inl"

#endif // _BUFFER_POOL_MANAGER_HPP_
