#pragma once

#ifndef _PAGE_HPP_
#define _PAGE_HPP_

#include <cstdint>

using page_id_t = int32_t;
struct Page
{
	static page_id_t generateId; // = 0; 
	page_id_t id{}; // = generateId++;
	char data[512]{}; // Stores the actual data

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
