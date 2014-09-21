#ifndef LANTERN_VECTOR4_H
#define LANTERN_VECTOR4_H

#include <cmath>

namespace lantern
{
	/** Class representing 4-dimensional homogeneous vector.
	*	Used to apply 3D affine transformations represented as 4x4 matrices.
	*/
	class vector4 final
	{
	public:
		float x;
		float y;
		float z;
		float w;

		vector4();
		vector4(float const x, float const y, float const z, float const w);
	};
}

#endif
