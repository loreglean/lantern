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

		/** Calculates matrix determinant */
		float det() const;

		/** Calculates matrix inverse */
		matrix3x3 inversed() const;

		/** Calculates matrix inverse using already calculated determinant to avoid calculating it twice */
		matrix3x3 inversed_precalc_det(float const det) const;

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
		static matrix3x3 rotation_around_axis(vector3f const& axis, float const radians);

		static const matrix3x3 IDENTITY;
	};

	template<typename T>
	vector3<T> operator*(vector3<T> const& v, matrix3x3 const& m)
	{
		return vector3<T>{
			v.x * m.values[0][0] + v.y * m.values[1][0] + v.z * m.values[2][0],
			v.x * m.values[0][1] + v.y * m.values[1][1] + v.z * m.values[2][1],
			v.x * m.values[0][2] + v.y * m.values[1][2] + v.z * m.values[2][2]};
	}
}

#endif
