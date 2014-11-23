#ifndef LANTERN_VECTOR2_H
#define LANTERN_VECTOR2_H

#include <cmath>

namespace lantern
{
	class vector2 final
	{
	public:
		float x;
		float y;

		vector2();
		vector2(float const x, float const y);
	};
}

#endif
