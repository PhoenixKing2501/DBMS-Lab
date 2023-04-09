#pragma once

#ifndef _TABLES_HPP_
#define _TABLES_HPP_

#include <array>
#include <compare>
#include <cstdint>

struct Books
{
	int32_t id;
	int32_t author_id;
	std::array<char, 28> title;
	std::array<char, 28> type;

	friend auto operator==(const Books &lhs, const Books &rhs) -> bool
	{
		return lhs.id == rhs.id;
	}

	friend auto operator<=>(const Books &lhs, const Books &rhs) -> std::strong_ordering
	{
		return lhs.id <=> rhs.id;
	}
};

struct Authors
{
	int32_t id;
	std::array<char, 30> fname;
	std::array<char, 30> lname;

	friend auto operator==(const Authors &lhs, const Authors &rhs) -> bool
	{
		return lhs.id == rhs.id;
	}

	friend auto operator<=>(const Authors &lhs, const Authors &rhs) -> std::strong_ordering
	{
		return lhs.id <=> rhs.id;
	}
};

constexpr int a = sizeof(Books);
constexpr int b = sizeof(Authors);

#endif // _TABLES_HPP_
