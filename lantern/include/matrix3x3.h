#ifndef LANTERN_MATRIX3X3_H
#define LANTERN_MATRIX3X3_H

#include <cmath>
#include "vector3.h"

namespace lantern
{
	/** Class representing 3x3 matrix.
	*	Coordinate system assumed to be left-handed. Values are accessed as [row][column].
	*/
	class matrix3x3 final
	{
	public:
		float values[3][3];

		matrix3x3();
		matrix3x3(float m[3][3]);
		matrix3x3(
			float const m00, float const m01, float const m02,
			float const m10, float const m11, float const m12,
			float const m20, float const m21, float const m22);

		matrix3x3 operator*(matrix3x3 const& m) const;

		static matrix3x3 scale(float const x, float const y, float const z);
		static matrix3x3 uniform_scale(float const s);
		static matrix3x3 rotation_around_x_axis(float const radians);
		static matrix3x3 rotation_around_y_axis(float const radians);
		static matrix3x3 rotation_around_z_axis(float const radians);
		static matrix3x3 rotation_around_axis(vector3 const& axis, float const radians);

		static const matrix3x3 IDENTITY;
	};

	vector3 operator*(vector3 const& v, matrix3x3 const& m);
}

#endif
