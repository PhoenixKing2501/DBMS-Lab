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

#include "Types.hpp"

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
	 * @return The page, std::nullopt if the page does not exist
	 */
	auto read_page(page_id_t page_id) -> std::optional<Page>;

	/**
	 * @brief Add pages from a file to the disk
	 * @param filename The filename to read from
	 * @return The page ids of the pages added, std::nullopt if the file does not exist
	 */
	auto add_page(const std::string &filename) -> std::optional<std::vector<page_id_t>>;
};

using PDiskManager = DiskManager *;
using CPDiskManager = const DiskManager *;
using RDiskManager = DiskManager &;
using CRDiskManager = const DiskManager &;

#endif // _DISK_MANAGER_HPP_
