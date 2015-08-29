#include "color.h"

using namespace lantern;

const color color::BLACK = color{0.0f, 0.0f, 0.0f};
const color color::WHITE = color{1.0f, 1.0f, 1.0f};
const color color::RED = color{1.0f, 0.0f, 0.0f};
const color color::GREEN = color{0.0f, 1.0f, 0.0f};
const color color::BLUE = color{0.0f, 0.0f, 1.0f};

color color::with_alpha(float alpha) const
{
	return color{this->r, this->g, this->b, alpha};
}

bool color::operator==(color const& another) const
{
	return (equals(this->r, another.r) && equals(this->g, another.g) && equals(this->b, another.b) && equals(this->a, another.a));
}

bool color::operator!=(color const& another) const
{
	return !(*this == another);
}

color color::operator*(float const s) const
{
	return color{this->r * s, this->g * s, this->b * s, this->a * s};
}

color color::operator+(color const& another) const
{
	return color{this->r + another.r, this->g + another.g, this->b + another.b, this->a + another.a};
}
