#pragma once

#ifndef _BUFFER_POOL_MANAGER_HPP_
#define _BUFFER_POOL_MANAGER_HPP_

#include <array>
#include <cstdint>
#include <numeric>
#include <unordered_map>

#include "DiskManager.hpp"
#include "Page.hpp"
#include "Replacer.hpp"

using frame_id_t = int32_t;

template <size_t N = 10>
struct BufferPoolManager
{
	struct Frame
	{
		Page page{};
		bool is_pinned{false};
	};

	struct
	{
		std::array<frame_id_t, N> free_list{
			[&]() -> std::array<frame_id_t, N>
			{
				std::array<frame_id_t, N> free_list{};
				std::iota(std::begin(free_list), std::end(free_list), 0);
				return free_list;
			}()};
		size_t size{N};

		auto pop() -> frame_id_t { return free_list[--size]; }
		auto push(frame_id_t frame_id) -> void { free_list[size++] = frame_id; }
	} free_list{}; // The free list of frames in the buffer pool

	std::array<Frame, N> pages{};							// The buffer pool
	std::unordered_map<page_id_t, frame_id_t> page_table{}; // page_id -> frame_id
	PDiskManager disk_manager{nullptr};						// Don't delete these two... They should be deleted by the caller
	PReplacer replacer{nullptr};							// Don't delete these two... They should be deleted by the caller

	/**
	 * @brief Creates a new BufferPoolManager
	 * @param disk_manager disk manager instance
	 * @param replacer replacer instance
	 */
	explicit BufferPoolManager(PDiskManager disk_manager, PReplacer replacer);

	/**
	 * @brief Destroys the BufferPoolManager
	 */
	~BufferPoolManager() = default;

	/**
	 * @brief Fetches a page from the buffer pool
	 * @param page_id The page id to fetch
	 * @param[out] page The page fetched
	 * @return true if the page is fetched successfully, false otherwise
	 */
	auto fetch_page(page_id_t page_id, RPage page) -> bool;

	/**
	 * @brief Unpins a page
	 * @param page_id The page id to unpin
	 * @return true if the page is unpinned successfully, false otherwise
	 */
	auto unpin_page(page_id_t page_id) -> bool;

	/**
	 * @brief freees a page to disk
	 * @param page_id The page id to free
	 * @return true if the page is freed successfully, false otherwise
	 */
	auto free_page(page_id_t page_id) -> bool;
};

#endif // _BUFFER_POOL_MANAGER_HPP_
