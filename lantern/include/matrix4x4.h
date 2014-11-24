#ifndef LANTERN_MATRIX4X4_H
#define LANTERN_MATRIX4X4_H

#include <cmath>
#include "vector4.h"
#include "vector3.h"

namespace lantern
{
	/** Class representing 4x4 matrix.
	*	Used to represent 3D affine transformations.
	*	Coordinate system assumed to be left-handed. Values are accessed as [row][column].
	*/
	class matrix4x4 final
	{
	public:
		float values[4][4];

		matrix4x4();
		matrix4x4(float const m[4][4]);
		matrix4x4(
			float const m00, float const m01, float const m02, float const m03,
			float const m10, float const m11, float const m12, float const m13,
			float const m20, float const m21, float const m22, float const m23,
			float const m30, float const m31, float const m32, float const m33);

		matrix4x4 operator*(matrix4x4 const& m) const;

		static matrix4x4 translation(float const x, float const y, float const z);
		static matrix4x4 scale(float const x, float const y, float const z);
		static matrix4x4 uniform_scale(float const s);
		static matrix4x4 rotation_around_x_axis(float const radians);
		static matrix4x4 rotation_around_y_axis(float const radians);
		static matrix4x4 rotation_around_z_axis(float const radians);
		static matrix4x4 rotation_around_axis(vector3 const& axis, float const radians);
		static matrix4x4 clip_space(float const hfov, float const vfov, float const near, float const far);

		static const matrix4x4 IDENTITY;
	};

	// Inlined due to huge amount of invocations
	//
	inline vector4 operator*(vector4 const& v, matrix4x4 const& m)
	{
		return vector4{
			v.x * m.values[0][0] + v.y * m.values[1][0] + v.z * m.values[2][0] + v.w * m.values[3][0],
			v.x * m.values[0][1] + v.y * m.values[1][1] + v.z * m.values[2][1] + v.w * m.values[3][1],
			v.x * m.values[0][2] + v.y * m.values[1][2] + v.z * m.values[2][2] + v.w * m.values[3][2],
			v.x * m.values[0][3] + v.y * m.values[1][3] + v.z * m.values[2][3] + v.w * m.values[3][3]};
	}
}

#endif // LANTERN_MATRIX4X4_H
