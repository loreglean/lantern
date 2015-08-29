#ifndef LANTERN_LINE_H
#define LANTERN_LINE_H

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
		line(float const a_coeff, float const b_coeff, float const c_coeff);

		/** Constructs line with two points
		* @param x0 First point x coordinate
		* @param y0 First point y coordinate
		* @param x1 Second point x coordinate
		* @param y1 Second point y coordinate
		*/
		line(float const x0, float const y0, float const x1, float const y1);

		/** Calculates line equation value for a given point
		* @param x Point x-coordinate
		* @param y Point y-coordinate
		* @returns Line equation value
		*/
		float at(float const x, float const y) const;

		/** Calculates intersection point between two lines
		* @param line The other line
		* @returns Intersection point
		*/
		vector2f intersection(line const& line) const;
	};
}

#endif