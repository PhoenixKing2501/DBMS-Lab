#pragma once

#ifndef _TABLES_HPP_
#define _TABLES_HPP_

#include <cstdint>

struct Books
{
	int32_t id;
	int32_t author_id;
	char title[50];
	char type[50];
};

struct Authors
{
	int32_t id;
	char fname[50];
	char lname[50];
};

#endif // _TABLES_HPP_
