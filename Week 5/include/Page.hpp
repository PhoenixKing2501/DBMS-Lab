#pragma once

#ifndef _PAGE_HPP_
#define _PAGE_HPP_

#include <array>
#include <cstdint>

using page_id_t = int32_t;
constexpr size_t PAGE_SIZE = 512;

struct Page
{
	static page_id_t generateId;		// genrates the page id
	page_id_t id{};						// id of the page
	std::array<char, PAGE_SIZE> data{}; // Stores the actual data

	/**
	 * @brief Creates a new Page. The page id is automatically generated.
	 */
	Page()
	{
		id = generateId++;
	}
};

page_id_t Page::generateId = 0;

using PPage = Page *;
using CPPage = const Page *;
using RPage = Page &;
using CRPage = const Page &;

#endif // _PAGE_HPP_
