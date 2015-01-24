#ifndef LANTERN_GEOMETRY_UTILS_H
#define LANTERN_GEOMETRY_UTILS_H

#include <cmath>
#include "vector2.h"

namespace lantern
{
	/** Class representing line described with equation ax + by + c = 0 */
	class line final
	{
	public:
		/** X coefficient */
		float a;

		/** Y coefficient */
		float b;

		/** Free coefficient */
		float c;

		/** Constructs line with given coefficients
		* @param a_coeff X coefficient
		* @param b_coeff Y coefficient
		* @param c_coeff Free coefficient
		*/
		line(float const a_coeff, float const b_coeff, float const c_coeff)
			: a(a_coeff), b(b_coeff), c(c_coeff)
		{

		}

		/** Constructs line with two points
		* @param x0 First point x coordinate
		* @param y0 First point y coordinate
		* @param x1 Second point x coordinate
		* @param y1 Second point y coordinate
		*/
		line(float const x0, float const y0, float const x1, float const y1)
		{
			a = -(y1 - y0);
			b = x1 - x0;
			c = (y1 - y0) * x0 - (x1 - x0) * y0;
		}

		/** Calculates line equation value for a given point
		* @param x Point x-coordinate
		* @param y Point y-coordinate
		* @returns Line equation value
		*/
		float at(float const x, float const y) const
		{
			return x * a + y * b + c;
		}

		vector2f intersection(line const& line) const
		{
			float const y{(line.a * c - a * line.c) / (a * line.b - line.a * b)};
			float const x{a == 0.0f ? ((line.b * c - b * line.c) / (line.a * b)) : (-(b / a) * y - (c / a))};

			return vector2f{x, y};
		};
	};

	/** Class that represents axis-aligned bounding box */
	template<typename TPoint>
	class aabb final
	{
	public:
		/** Starting point */
		TPoint from;

		/** End point */
		TPoint to;
	};

	/** Calculates triangle area
	* @param a First triangle point
	* @param b Second triangle point
	* @param c Third triangle point
	* @returns Triangle area
	*/
	inline float triangle_2d_area(
		float const x0, float const y0,
		float const x1, float const y1,
		float const x2, float const y2)
	{
		return std::abs(0.5f * (x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)));
	}
}

#endif // LANTERN_GEOMETRY_UTILS_H