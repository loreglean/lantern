#ifndef LATNERN_COMMON_MATH_H
#define LATNERN_COMMON_MATH_H

#include <cmath>
#include "vector2.h"

namespace lantern
{
	float const FLOAT_EPSILON = 0.00001f;
	
	template<typename T>
	inline bool equals(T const first, T const second)
	{
		return (first == second);
	}

	template<>
	inline bool equals<float>(float const first, float const second)
	{
		return std::abs(first - second) < FLOAT_EPSILON;
	}

	/** Calculates triangle area
	* @param a First triangle point
	* @param b Second triangle point
	* @param c Third triangle point
	* @returns Triangle area
	*/
	float triangle_2d_area(
		float const x0, float const y0,
		float const x1, float const y1,
		float const x2, float const y2);

	float clamp(float const value, float const from, float const to);
}

#endif // LATNERN_COMMON_MATH_H