#pragma once

#ifndef _PAGE_HPP_
#define _PAGE_HPP_

#include <cstdint>

struct Page
{
	static int32_t generateId;
	int32_t id{};
	char data[1024]{};

	Page()
	{
		id = generateId++;
	}
};

int32_t Page::generateId = 0;

using PPage = Page *;
using CPPage = const Page *;
using RPage = Page &;
using CRPage = const Page &;

#endif // _PAGE_HPP_
