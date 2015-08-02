#ifndef LANTERN_MATRIX4X4_H
#define LANTERN_MATRIX4X4_H

#include <cmath>
#include "vector4.h"
#include "vector3.h"

namespace lantern
{
	/** Class representing 4x4 matrix.
	* Used to represent 3D affine transformations.
	* Coordinate system assumed to be left-handed
	*/
	class matrix4x4 final
	{
	public:
		/** Data array, [row][column] */
		float values[4][4];

		/** Constructs matrix with zero values */
		matrix4x4();

		/** Constructs matrix using given array
		* @param array Matrix values array
		*/
		matrix4x4(float const m[4][4]);

		/** Constructs matrix with specified values */
		matrix4x4(
			float const m00, float const m01, float const m02, float const m03,
			float const m10, float const m11, float const m12, float const m13,
			float const m20, float const m21, float const m22, float const m23,
			float const m30, float const m31, float const m32, float const m33);

		// Operators
		//

		matrix4x4 operator*(matrix4x4 const& m) const;

		/** Generates translation matrix
		* @param x Distance along x-axis
		* @param y Distance along y-axis
		* @param z Distance along z-axis
		*/
		static matrix4x4 translation(float const x, float const y, float const z);

		/** Generates scale matrix
		* @param x Scale along x-axis
		* @param y Scale along y-axis
		* @param z Scale along z-axis
		*/
		static matrix4x4 scale(float const x, float const y, float const z);

		/** Generates uniform scale matrix
		* @param s Scale along each axis
		*/
		static matrix4x4 uniform_scale(float const s);

		/** Generates rotation around x-axis
		* @param radians Radians to rotate for
		*/
		static matrix4x4 rotation_around_x_axis(float const radians);

		/** Generates rotation around y-axis
		* @param radians Radians to rotate for
		*/
		static matrix4x4 rotation_around_y_axis(float const radians);

		/** Generates rotation around z-axis
		* @param radians Radians to rotate for
		*/
		static matrix4x4 rotation_around_z_axis(float const radians);

		/** Generates rotation around specified axis
		* @param axis Axis to rotate around
		* @param radians Radians to rotate for
		*/
		static matrix4x4 rotation_around_axis(vector3f const& axis, float const radians);

		/** Generates matrix that transforms point from camera space to homogeneous clip space
		* @param hfov Horizontal field of view
		* @param vfov Vertical field of view
		* @param near Near plane z-coordinate
		* @param far Far plane z-coordinate
		*/
		static matrix4x4 clip_space(float const hfov, float const vfov, float const near, float const far);

		static const matrix4x4 IDENTITY;
	};

	inline vector4f operator*(vector4f const& v, matrix4x4 const& m)
	{
		return vector4f{
			v.x * m.values[0][0] + v.y * m.values[1][0] + v.z * m.values[2][0] + v.w * m.values[3][0],
			v.x * m.values[0][1] + v.y * m.values[1][1] + v.z * m.values[2][1] + v.w * m.values[3][1],
			v.x * m.values[0][2] + v.y * m.values[1][2] + v.z * m.values[2][2] + v.w * m.values[3][2],
			v.x * m.values[0][3] + v.y * m.values[1][3] + v.z * m.values[2][3] + v.w * m.values[3][3]};
	}
}

#endif // LANTERN_MATRIX4X4_H
