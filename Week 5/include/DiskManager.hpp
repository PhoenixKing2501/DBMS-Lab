#pragma once

#ifndef _DISKMANAGER_HPP_
#define _DISKMANAGER_HPP_

// #include <array>
#include <fstream>
#include <string>
#include <vector>

#include "Page.hpp"

struct DiskManager
{
	std::string filename;
	std::fstream file;
	std::vector<Page> pages;

	DiskManager(const std::string &filename);
	~DiskManager();

	// void open();
	// void close();

	/**
	 * @brief Read a page from disk
	 * @param page_id The page id to read from disk
	 * @param[out] page_data The page data read from disk
	 */
	void read(int32_t page_id, char *page_data);
};

#endif // _DISKMANAGER_HPP_
