#pragma once

#ifndef _DISK_MANAGER_HPP_
#define _DISK_MANAGER_HPP_

// #include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "Page.hpp"

struct DiskManager
{
	std::string filename{};	   // The filename of the disk
	std::fstream file{};	   // The file stream of the disk
	std::vector<Page> pages{}; // The pages on the disk
	int32_t num_ios{0};		   // The number of I/Os

	/**
	 * @brief Creates a new DiskManager with the given filename
	 * @param filename The filename of the diskfile
	 */
	DiskManager(const std::string &filename);
	~DiskManager();

	// void open();
	// void close();

	/**
	 * @brief Read a page from disk
	 * @param page_id The page id to read from disk
	 * @param[out] page_data The page data read from disk
	 */
	auto read(int32_t page_id, char *page_data) -> void;

	/**
	 * @brief Add a page to the disk
	 * @param page The page to add to the disk
	 * @return The page id of the page added
	 */
	auto add_page(const Page &page) -> int32_t;
};

using PDiskManager = DiskManager *;
using CPDiskManager = const DiskManager *;
using RDiskManager = DiskManager &;
using CRDiskManager = const DiskManager &;

#endif // _DISK_MANAGER_HPP_
