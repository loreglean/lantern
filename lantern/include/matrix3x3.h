#ifndef LANTERN_MATRIX3X3_H
#define LANTERN_MATRIX3X3_H

#include <cmath>
#include "vector3.h"

namespace lantern
{
	/** Class representing 3x3 matrix.
	* Coordinate system assumed to be left-handed
	*/
	class matrix3x3 final
	{
	public:
		/** Data array, [row][column] */
		float values[3][3];

		/** Constructs matrix with zero values */
		matrix3x3();

		/** Constructs matrix using given array
		* @param array Matrix values array
		*/
		matrix3x3(float const array[3][3]);

		/** Constructs matrix with specified values */
		matrix3x3(
			float const m00, float const m01, float const m02,
			float const m10, float const m11, float const m12,
			float const m20, float const m21, float const m22);

		matrix3x3 operator*(matrix3x3 const& m) const;

		/** Generates scale matrix
		* @param x Scale along x-axis
		* @param y Scale along y-axis
		* @param z Scale along z-axis
		*/
		static matrix3x3 scale(float const x, float const y, float const z);

		/** Generates uniform scale matrix
		* @param s Scale along each axis
		*/
		static matrix3x3 uniform_scale(float const s);

		/** Generates rotation around x-axis
		* @param radians Radians to rotate for
		*/
		static matrix3x3 rotation_around_x_axis(float const radians);

		/** Generates rotation around y-axis
		* @param radians Radians to rotate for
		*/
		static matrix3x3 rotation_around_y_axis(float const radians);

		/** Generates rotation around z-axis
		* @param radians Radians to rotate for
		*/
		static matrix3x3 rotation_around_z_axis(float const radians);

		/** Generates rotation around specified axis
		* @param axis Axis to rotate around
		* @param radians Radians to rotate for
		*/
		static matrix3x3 rotation_around_axis(vector3 const& axis, float const radians);

		static const matrix3x3 IDENTITY;
	};

	vector3 operator*(vector3 const& v, matrix3x3 const& m);
}

#endif
