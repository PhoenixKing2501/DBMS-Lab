#pragma once

#ifndef _TABLES_HPP_
#define _TABLES_HPP_

#include <array>
#include <compare>
#include <cstdint>

struct Employee
{
	int32_t id;
	int32_t company_id;
	std::array<char, 60> fname;
	std::array<char, 60> lname;

	friend auto operator==(const Employee &lhs, const Employee &rhs) -> bool
	{
		return lhs.company_id == rhs.company_id;
	}

	friend auto operator<=>(const Employee &lhs, const Employee &rhs) -> std::strong_ordering
	{
		return lhs.company_id <=> rhs.company_id;
	}
};

struct Company
{
	int32_t id;
	std::array<char, 62> name;
	std::array<char, 62> slogan;

	friend auto operator==(const Company &lhs, const Company &rhs) -> bool
	{
		return lhs.id == rhs.id;
	}

	friend auto operator<=>(const Company &lhs, const Company &rhs) -> std::strong_ordering
	{
		return lhs.id <=> rhs.id;
	}
};

constexpr int a = sizeof(Employee);
constexpr int b = sizeof(Company);

#endif // _TABLES_HPP_
