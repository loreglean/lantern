#include "math_common.h"

using namespace lantern;

float lantern::triangle_2d_area(
	float const x0, float const y0,
	float const x1, float const y1,
	float const x2, float const y2)
{
	return std::abs(0.5f * (x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)));
}

float lantern::clamp(float const value, float const from, float const to)
{
	return (value > to ? to : (value < from ? from : value));
}