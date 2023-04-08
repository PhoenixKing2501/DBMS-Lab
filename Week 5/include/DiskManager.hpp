#pragma once

#ifndef _DISK_MANAGER_HPP_
#define _DISK_MANAGER_HPP_

// #include <array>
#include <array>
#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>

#include "Page.hpp"

struct DiskManager
{
	std::unordered_map<page_id_t, Page> pages{}; // The pages on the disk
	int64_t num_ios{0};							 // The number of I/Os

	DiskManager() = default;
	~DiskManager() = default;

	// void open();
	// void close();

	/**
	 * @brief Read a page from disk
	 * @param page_id The page id to read from disk
	 * @param[out] page_data The page data read from disk
	 */
	auto read(page_id_t page_id) -> std::optional<std::array<char, PAGE_SIZE>>;

	/**
	 * @brief Add pages from a file to the disk
	 * @param filename The filename to read from
	 */
	auto add_page(const std::string &filename) -> void;
};

using PDiskManager = DiskManager *;
using CPDiskManager = const DiskManager *;
using RDiskManager = DiskManager &;
using CRDiskManager = const DiskManager &;

#endif // _DISK_MANAGER_HPP_
