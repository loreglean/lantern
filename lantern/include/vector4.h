#ifndef LANTERN_VECTOR4_H
#define LANTERN_VECTOR4_H

#include <cmath>

namespace lantern
{
	/** Class representing 4-dimensional homogeneous vector.
	* Used to apply 3D affine transformations represented as 4x4 matrices
	*/
	class vector4 final
	{
	public:
		/** X-coordinate */
		float x;

		/** Y-coordinate */
		float y;

		/** Z-coordinate */
		float z;

		/** W-coordinate */
		float w;

		/** Constructs vector with zero coordinates */
		vector4();

		/** Constructs vector with specified coordinates values */
		vector4(float const x, float const y, float const z, float const w);
	};
}

#endif
