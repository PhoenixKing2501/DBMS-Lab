#pragma once

#ifndef _TABLES_HPP_
#define _TABLES_HPP_

#include <cstdint>

struct Books
{
	int32_t id;
	int32_t author_id;
	char title[28];
	char type[28];
};

struct Authors
{
	int32_t id;
	char fname[30];
	char lname[30];
};

constexpr int a = sizeof(Books);
constexpr int b = sizeof(Authors);

#endif // _TABLES_HPP_
