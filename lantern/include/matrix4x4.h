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
		matrix4x4(float m[4][4]);
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

		static const matrix4x4 IDENTITY;
    };

	vector4 operator*(vector4 const& v, matrix4x4 const& m);
}

#endif // LANTERN_MATRIX4X4_H
