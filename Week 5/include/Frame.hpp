#pragma once

#ifndef _FRAME_HPP_
#define _FRAME_HPP_

#include "Page.hpp"

struct Frame
{
	Page page{};		   // The page stored in the frame
	uint16_t pin_count{0}; // The number of pins on the page
};

using PFrame = Frame *;
using CPFrame = const Frame *;
using RFrame = Frame &;
using CRFrame = const Frame &;

#endif // _FRAME_HPP_
