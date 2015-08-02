#include "color.h"

using namespace lantern;

const color color::BLACK = color{0.0f, 0.0f, 0.0f};
const color color::WHITE = color{1.0f, 1.0f, 1.0f};
const color color::RED = color{1.0f, 0.0f, 0.0f};
const color color::GREEN = color{0.0f, 1.0f, 0.0f};
const color color::BLUE = color{0.0f, 0.0f, 1.0f};

color color::operator*(float const s) const
{
	return color{r * s, g * s, b * s};
}

color color::operator+(color const& c) const
{
	return color{r + c.r, g + c.g, b + c.b};
}

bool color::operator==(color const& c) const
{
	return (equals(this->r, c.r) && equals(this->g, c.g) && equals(this->b, c.b));
}

bool color::operator!=(color const& c) const
{
	return !(*this == c);
}