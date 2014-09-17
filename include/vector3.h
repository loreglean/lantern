#ifndef LANTERN_VECTOR3_H
#define LANTERN_VECTOR3_H

#include <cmath>

namespace lantern
{
	/** Class representing 3-dimensional vector.
	*	Can be interpreted either as a point or a geometrical vector,
	*	providing operations for both interpretations.
	*	It also can be used to just store three related float values.
	*/
	class vector3
	{
	public:
		float x;
		float y;
		float z;

		vector3();
		vector3(float const x, float const y, float const z);
		vector3 operator+(vector3 const& v) const;
		vector3& operator+=(vector3 const& v);
		vector3 operator-(vector3 const& v) const;
		vector3& operator-=(vector3 const& v);
		vector3 operator-() const;
		vector3 operator*(float const s) const;
		vector3& operator*=(float const s);
		vector3 operator/(float const s) const;
		vector3& operator/=(float const s);

		float length() const;
		float length_sqr() const;
		void normalize();
		vector3 normalized() const;
		float dot(vector3 const& v) const;
		vector3 cross(vector3 const& v) const;
		float distance_to(vector3 const& p) const;
		float angle_with(vector3 const& v) const;
		vector3 projection_on(vector3 const& v) const;
		vector3 perpendicular_on(vector3 const& v) const;

		static vector3 const ZERO;
		static vector3 const X_UNIT;
		static vector3 const Y_UNIT;
		static vector3 const Z_UNIT;
	};

	inline vector3::vector3()
		: x{0.0f}, y{0.0f}, z{0.0f}
	{

	}

	inline vector3::vector3(float const x, float const y, float const z)
		: x{x}, y{y}, z{z}
	{

	}

	inline vector3 vector3::operator+(vector3 const& v) const
	{
		return vector3{x + v.x, y + v.y, z + v.z};
	}

	inline vector3& vector3::operator+=(vector3 const& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	inline vector3 vector3::operator-(vector3 const& v) const
	{
		return vector3{x - v.x, y - v.y, z - v.z};
	}

	inline vector3& vector3::operator-=(vector3 const& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	inline vector3 vector3::operator-() const
	{
		return vector3{-x, -y, -z};
	}

	inline vector3 vector3::operator*(float const s) const
	{
		return vector3{x * s, y * s, z * s};
	}

	inline vector3& vector3::operator*=(float const s)
	{
		x *= s;
		y *= s;
		z *= s;

		return *this;
	}

	inline vector3 vector3::operator/(float const s) const
	{
		return vector3{x / s, y / s, z / s};
	}

	inline vector3& vector3::operator/=(float const s)
	{
		x /= s;
		y /= s;
		z /= s;

		return *this;
	}

	inline float vector3::length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	inline float vector3::length_sqr() const
	{
		return x * x + y * y + z * z;
	}

	inline void vector3::normalize()
	{
		float const length_reciprocal{1.0f / length()};

		x *= length_reciprocal;
		y *= length_reciprocal;
		z *= length_reciprocal;
	}

	inline vector3 vector3::normalized() const
	{
		float const length_reciprocal{1.0f / length()};

		return vector3{x * length_reciprocal, y * length_reciprocal, z * length_reciprocal};
	}

	inline float vector3::distance_to(vector3 const& p) const
	{
		float const dist_x{p.x - x};
		float const dist_y{p.y - y};
		float const dist_z{p.z - z};
		return std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
	}

	inline float vector3::angle_with(vector3 const &v) const
	{
		return std::acos(dot(v) / (length() * v.length()));
	}

	inline vector3 vector3::projection_on(vector3 const &v) const
	{
		return v.normalized() * (dot(v) / v.length());
	}

	inline vector3 vector3::perpendicular_on(vector3 const &v) const
	{
		return (*this) - projection_on(v);
	}

	inline float vector3::dot(vector3 const& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline vector3 vector3::cross(vector3 const& v) const
	{
		return vector3{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
	}

	inline vector3 operator*(float const s, vector3 const v)
	{
		return vector3{v.x * s, v.y * s, v.z * s};
	}
}

#endif // LANTERN_VECTOR3_H
