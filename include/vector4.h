#ifndef LANTERN_VECTOR4_H
#define LANTERN_VECTOR4_H

#include <cmath>

namespace lantern
{
	/** Class representing 4-dimensional homogeneous vector.
	*	Used to apply 3D affine transformations represented as 4x4 matrices.
	*/
	class vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		vector4();
		vector4(float const x, float const y, float const z, float const w);
	};

	inline vector4::vector4()
		: x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f}
	{
	}

	inline vector4::vector4(float const x, float const y, float const z, float const w)
		: x{x}, y{y}, z{z}, w{w}
	{

	}
}

#endif
