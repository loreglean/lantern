#include "vector3.h"

using namespace lantern;

vector3 const vector3::ZERO = vector3{0.0f, 0.0f, 0.0f};
vector3 const vector3::X_UNIT = vector3{1.0f, 0.0f, 0.0f};
vector3 const vector3::Y_UNIT = vector3{0.0f, 1.0f, 0.0f};
vector3 const vector3::Z_UNIT = vector3{0.0f, 0.0f, 1.0f};

vector3::vector3()
	: x{0.0f}, y{0.0f}, z{0.0f}
{

}

vector3::vector3(float const x, float const y, float const z)
	: x{x}, y{y}, z{z}
{

}

vector3 vector3::operator+(vector3 const& v) const
{
	return vector3{x + v.x, y + v.y, z + v.z};
}

vector3& vector3::operator+=(vector3 const& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

vector3 vector3::operator-(vector3 const& v) const
{
	return vector3{x - v.x, y - v.y, z - v.z};
}

vector3& vector3::operator-=(vector3 const& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

vector3 vector3::operator-() const
{
	return vector3{-x, -y, -z};
}

vector3 vector3::operator*(float const s) const
{
	return vector3{x * s, y * s, z * s};
}

vector3& vector3::operator*=(float const s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

vector3 vector3::operator/(float const s) const
{
	return vector3{x / s, y / s, z / s};
}

vector3& vector3::operator/=(float const s)
{
	x /= s;
	y /= s;
	z /= s;

	return *this;
}

float vector3::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

float vector3::length_sqr() const
{
	return x * x + y * y + z * z;
}

void vector3::normalize()
{
	float const length_reciprocal{1.0f / length()};

	x *= length_reciprocal;
	y *= length_reciprocal;
	z *= length_reciprocal;
}

vector3 vector3::normalized() const
{
	float const length_reciprocal{1.0f / length()};

	return vector3{x * length_reciprocal, y * length_reciprocal, z * length_reciprocal};
}

float vector3::distance_to(vector3 const& p) const
{
	float const dist_x{p.x - x};
	float const dist_y{p.y - y};
	float const dist_z{p.z - z};
	return std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
}

float vector3::angle_with(vector3 const& v) const
{
	return std::acos(dot(v) / (length() * v.length()));
}

vector3 vector3::projection_on(vector3 const& v) const
{
	return v.normalized() * (dot(v) / v.length());
}

vector3 vector3::perpendicular_on(vector3 const& v) const
{
	return (*this) - projection_on(v);
}

float vector3::dot(vector3 const& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

vector3 vector3::cross(vector3 const& v) const
{
	return vector3{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

vector3 lantern::operator*(float const s, vector3 const& v)
{
	return vector3{v.x * s, v.y * s, v.z * s};
}
