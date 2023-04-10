#pragma once

#ifndef _PAGE_HPP_
#define _PAGE_HPP_

#include <array>
#include <functional>

#include "Types.hpp"

struct Page
{
	static page_id_t generateId;		// genrates the page id
	page_id_t id{};						// id of the page
	std::array<char, PAGE_SIZE> data{}; // Stores the actual data

	static std::function<page_id_t()> generate_page_id;
};

using PPage = Page *;
using CPPage = const Page *;
using RPage = Page &;
using CRPage = const Page &;

#endif // _PAGE_HPP_
