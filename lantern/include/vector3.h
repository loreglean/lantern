#ifndef LANTERN_VECTOR3_H
#define LANTERN_VECTOR3_H

#include <cmath>

namespace lantern
{
	/** Simple 3-dimensional vector */
	template<typename T>
	class vector3 final
	{
	public:
		/** X-coordinate */
		T x;

		/** Y-coordinate */
		T y;

		/** Z-coordinate */
		T z;

		/** Constructs vector with zero values */
		vector3();

		/** Constructs vector with specified values
		* @param x X-coordinate
		* @param y Y-coordinate
		* @param z Z-coordinate
		*/
		vector3(T const x, T const y, T const z);

		vector3<T> operator+(vector3<T> const& v) const;
		vector3<T>& operator+=(vector3<T> const& v);
		vector3<T> operator-(vector3<T> const& v) const;
		vector3<T>& operator-=(vector3<T> const& v);
		vector3<T> operator-() const;
		vector3<T> operator*(float const s) const;
		vector3<T>& operator*=(float const s);
		vector3<T> operator/(float const s) const;
		vector3<T>& operator/=(float const s);

		/** Calculates vector's length
		* @returns Length
		*/
		float length() const;

		/** Calculates vector's squared length, thus avoiding square root operation
		* @returns Squared length
		*/
		float length_sqr() const;

		/** Normalizes vector (changing length to 1) */
		void normalize();

		/** Gets normalized vector, leaving this vector untouched
		* @returns Normalized version of this vector
		*/
		vector3<T> normalized() const;

		/** Calculates dot product with another vector
		* @returns Dot product
		*/
		float dot(vector3<T> const& v) const;

		/** Calculates cross product with another vector
		* @returns Cross product
		*/
		vector3 cross(vector3<T> const& v) const;

		/** Calculates distance to another point
		* @returns Distance
		*/
		float distance_to(vector3<T> const& p) const;

		/** Calculates angle with another vector
		* @returns Angle in radians
		*/
		float angle_with(vector3<T> const& v) const;

		/** Calculates projection of this vector on another vector
		* @returns Projection vector
		*/
		vector3<T> projection_on(vector3<T> const& v) const;

		/** Calculates perpendicular from this vector to another vector
		* @returns Perpendicular
		*/
		vector3<T> perpendicular_on(vector3 const& v) const;
	};

	template<typename T>
	vector3<T>::vector3()
		: x{}, y{}, z{}
	{

	}

	template<typename T>
	vector3<T>::vector3(T const x, T const y, T const z)
		: x(x), y(y), z(z)
	{

	}

	template<typename T>
	vector3<T> vector3<T>::operator+(vector3<T> const& v) const
	{
		return vector3<T>{x + v.x, y + v.y, z + v.z};
	}

	template<typename T>
	vector3<T>& vector3<T>::operator+=(vector3<T> const& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	template<typename T>
	vector3<T> vector3<T>::operator-(vector3<T> const& v) const
	{
		return vector3{x - v.x, y - v.y, z - v.z};
	}

	template<typename T>
	vector3<T>& vector3<T>::operator-=(vector3<T> const& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	template<typename T>
	vector3<T> vector3<T>::operator-() const
	{
		return vector3<T>{-x, -y, -z};
	}

	template<typename T>
	vector3<T> vector3<T>::operator*(float const s) const
	{
		return vector3<T>{x * s, y * s, z * s};
	}

	template<typename T>
	vector3<T>& vector3<T>::operator*=(float const s)
	{
		x *= s;
		y *= s;
		z *= s;

		return *this;
	}

	template<typename T>
	vector3<T> vector3<T>::operator/(float const s) const
	{
		return vector3{x / s, y / s, z / s};
	}

	template<typename T>
	vector3<T>& vector3<T>::operator/=(float const s)
	{
		x /= s;
		y /= s;
		z /= s;

		return *this;
	}

	template<typename T>
	float vector3<T>::length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	template<typename T>
	float vector3<T>::length_sqr() const
	{
		return x * x + y * y + z * z;
	}

	template<typename T>
	void vector3<T>::normalize()
	{
		float const length_reciprocal{1.0f / length()};

		x *= length_reciprocal;
		y *= length_reciprocal;
		z *= length_reciprocal;
	}

	template<typename T>
	vector3<T> vector3<T>::normalized() const
	{
		float const length_reciprocal{1.0f / length()};

		return vector3<T>{x * length_reciprocal, y * length_reciprocal, z * length_reciprocal};
	}

	template<typename T>
	float vector3<T>::distance_to(vector3<T> const& p) const
	{
		float const dist_x{p.x - x};
		float const dist_y{p.y - y};
		float const dist_z{p.z - z};
		return std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
	}

	template<typename T>
	float vector3<T>::angle_with(vector3<T> const& v) const
	{
		return std::acos(dot(v) / (length() * v.length()));
	}

	template<typename T>
	vector3<T> vector3<T>::projection_on(vector3<T> const& v) const
	{
		return v.normalized() * (dot(v) / v.length());
	}

	template<typename T>
	vector3<T> vector3<T>::perpendicular_on(vector3<T> const& v) const
	{
		return (*this) - projection_on(v);
	}

	template<typename T>
	float vector3<T>::dot(vector3<T> const& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	template<typename T>
	vector3<T> vector3<T>::cross(vector3<T> const& v) const
	{
		return vector3<T>{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
	}

	template<typename T>
	vector3<T> operator*(float const s, vector3<T> const& v)
	{
		return vector3<T>{v.x * s, v.y * s, v.z * s};
	}

	typedef vector3<float> vector3f;
}

#endif // LANTERN_VECTOR3_H
