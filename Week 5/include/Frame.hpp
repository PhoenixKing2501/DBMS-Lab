#pragma once

#ifndef _FRAME_HPP_
#define _FRAME_HPP_

#include "Page.hpp"

struct Frame
{
	Page page{};		   // The page stored in the frame
	bool is_pinned{false}; // Whether the page is pinned or not
};

#endif // _FRAME_HPP_
