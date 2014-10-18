#include "point.h"

using namespace lantern;

bool point2d::operator==(point2d const &p) const
{
	return ((x == p.x) && (y == p.y));
}
