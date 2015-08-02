#include "line.h"

using namespace lantern;

line::line(float const a_coeff, float const b_coeff, float const c_coeff)
	: a{a_coeff}, b{b_coeff}, c{c_coeff}
{

}

line::line(float const x0, float const y0, float const x1, float const y1)
{
	a = -(y1 - y0);
	b = x1 - x0;
	c = (y1 - y0) * x0 - (x1 - x0) * y0;
}

float line::at(float const x, float const y) const
{
	return x * a + y * b + c;
}

vector2f line::intersection(line const& line) const
{
	float const y{(line.a * c - a * line.c) / (a * line.b - line.a * b)};
	float const x{a == 0.0f ? ((line.b * c - b * line.c) / (line.a * b)) : (-(b / a) * y - (c / a))};

	return vector2f{x, y};
}