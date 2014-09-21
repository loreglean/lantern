#include "point.h"

using namespace lantern;

bool point2D::operator==(point2D const &p) const
{
	return ((x == p.x) && (y == p.y));
}
