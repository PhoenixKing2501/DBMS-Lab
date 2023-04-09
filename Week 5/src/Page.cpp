#include "Page.hpp"

page_id_t Page::generateId = 0;
std::function<page_id_t()> Page::generate_page_id = []() -> page_id_t
{
	return generateId++;
};
