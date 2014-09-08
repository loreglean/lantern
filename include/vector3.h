#ifndef LANTERN_VECTOR3_H
#define LANTERN_VECTOR3_H

#include <cmath>

namespace lantern
{
	class vector3
	{
	public:

		float x;
		float y;
		float z;

		vector3(float x, float y, float z)
			: x(x), y(y), z(z)
		{

		}

		vector3 operator-() const
		{
			return vector3(-x, -y, -z);
		}

		vector3 operator*(float s) const
		{
			return vector3(x * s, y * s, z * s);
		}

		vector3& operator*=(float s)
		{
			x *= s;
			y *= s;
			z *= s;

			return *this;
		}

		vector3 operator+(vector3 const& v) const
		{
			return vector3(x + v.x, y + v.y, z + v.z);
		}

		vector3& operator+=(vector3 const& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;

			return *this;
		}

		vector3 operator-(vector3 const& v) const
		{
			return vector3(x - v.x, y - v.y, z - v.z);
		}

		vector3& operator-=(vector3 const& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		float length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		void normalize()
		{
			float length_reciprocal = 1.0f / length();

			x *= length_reciprocal;
			y *= length_reciprocal;
			z *= length_reciprocal;
		}

		float dot(vector3 const& v) const
		{
			return (x * v.x + y * v.y + z * v.z);
		}

		vector3 cross(vector3 const& v) const
		{
			return vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}

		float distance_to(vector3 const& p) const
		{
			float dist_x = p.x - x;
			float dist_y = p.y - y;
			float dist_z = p.z - z;
			return sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
		}

		static vector3 const ZERO;
		static vector3 const X_UNIT;
		static vector3 const Y_UNIT;
		static vector3 const Z_UNIT;
	};

	vector3 operator*(float s, vector3 v)
	{
		return vector3(v.x * s, v.y * s, v.z * s);
	}
}

#endif
