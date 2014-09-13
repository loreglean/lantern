#ifndef LANTERN_VECTOR4_H
#define LANTERN_VECTOR4_H

#include <cmath>

namespace lantern
{
	class vector4
	{
	public:

		float x;
		float y;
		float z;
		float w;

		vector4(float const x, float const y, float const z, float const w)
			: x(x), y(y), z(z), w(w)
		{

		}
	};
}

#endif
