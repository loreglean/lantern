#ifndef LANTERN_VECTOR3_H
#define LANTERN_VECTOR3_H

#include "math_common.h"

namespace lantern
{
	/** 3-dimensional vector template class */
	template<typename T>
	class vector3 final
	{
	public:
		/** X-coordinate of vector */
		T x;

		/** Y-coordinate of vector */
		T y;

		/** Z-coordinate of vector */
		T z;

		/** Default constructor (initializes with default values) */
		vector3();

		/** Constructs vector with specified values
		* @param x X-coordinate of a vector
		* @param y Y-coordinate of a vector
		* @param z Z-coordinate of a vector
		*/
		vector3(T const x, T const y, T const z);

		// Operators
		//

		bool operator==(vector3<T> const& v) const;
		vector3<T> operator+(vector3<T> const& v) const;
		vector3<T>& operator+=(vector3<T> const& v);
		vector3<T> operator-(vector3<T> const& v) const;
		vector3<T>& operator-=(vector3<T> const& v);
		vector3<T> operator-() const;
		vector3<T> operator*(float const f) const;
		vector3<T>& operator*=(float const f);
		vector3<T> operator/(float const f) const;
		vector3<T>& operator/=(float const f);

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

	// vector3<T> implementation
	//

	template<typename T>
	inline vector3<T>::vector3()
		: x{}, y{}, z{}
	{

	}

	template<typename T>
	inline vector3<T>::vector3(T const x, T const y, T const z)
		: x(x), y(y), z(z)
	{

	}

	template<typename T>
	inline bool vector3<T>::operator==(vector3<T> const& v) const
	{
		return equals(this->x, v.x) && equals(this->y, v.y) && equals(this->z, v.z);
	}

	template<typename T>
	inline vector3<T> vector3<T>::operator+(vector3<T> const& v) const
	{
		return vector3<T>{this->x + v.x, this->y + v.y, this->z + v.z};
	}

	template<typename T>
	inline vector3<T>& vector3<T>::operator+=(vector3<T> const& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}

	template<typename T>
	inline vector3<T> vector3<T>::operator-(vector3<T> const& v) const
	{
		return vector3{this->x - v.x, this->y - v.y, this->z - v.z};
	}

	template<typename T>
	inline vector3<T>& vector3<T>::operator-=(vector3<T> const& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	template<typename T>
	inline vector3<T> vector3<T>::operator-() const
	{
		return vector3<T>{-this->x, -this->y, -this->z};
	}

	template<typename T>
	inline vector3<T> vector3<T>::operator*(float const f) const
	{
		return vector3<T>{this->x * f, this->y * f, this->z * f};
	}

	template<typename T>
	inline vector3<T>& vector3<T>::operator*=(float const f)
	{
		this->x *= f;
		this->y *= f;
		this->z *= f;

		return *this;
	}

	template<typename T>
	inline vector3<T> vector3<T>::operator/(float const f) const
	{
		return vector3{this->x / f, this->y / f, this->z / f};
	}

	template<typename T>
	inline vector3<T>& vector3<T>::operator/=(float const f)
	{
		this->x /= f;
		this->y /= f;
		this->z /= f;

		return *this;
	}

	template<typename T>
	inline float vector3<T>::length() const
	{
		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	template<typename T>
	inline float vector3<T>::length_sqr() const
	{
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	template<typename T>
	inline void vector3<T>::normalize()
	{
		float const length_reciprocal{1.0f / this->length()};

		this->x *= length_reciprocal;
		this->y *= length_reciprocal;
		this->z *= length_reciprocal;
	}

	template<typename T>
	inline vector3<T> vector3<T>::normalized() const
	{
		float const length_reciprocal{1.0f / this->length()};

		return vector3<T>{this->x * length_reciprocal, this->y * length_reciprocal, this->z * length_reciprocal};
	}

	template<typename T>
	inline float vector3<T>::distance_to(vector3<T> const& v) const
	{
		float const dist_x{v.x - this->x};
		float const dist_y{v.y - this->y};
		float const dist_z{v.z - this->z};

		return std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
	}

	template<typename T>
	inline float vector3<T>::angle_with(vector3<T> const& v) const
	{
		return std::acos(this->dot(v) / (this->length() * v.length()));
	}

	template<typename T>
	inline vector3<T> vector3<T>::projection_on(vector3<T> const& v) const
	{
		return v.normalized() * (this->dot(v) / v.length());
	}

	template<typename T>
	inline vector3<T> vector3<T>::perpendicular_on(vector3<T> const& v) const
	{
		return (*this) - this->projection_on(v);
	}

	template<typename T>
	inline float vector3<T>::dot(vector3<T> const& v) const
	{
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

	template<typename T>
	inline vector3<T> vector3<T>::cross(vector3<T> const& v) const
	{
		return vector3<T>{this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x};
	}

	template<typename T>
	inline vector3<T> operator*(float const f, vector3<T> const& v)
	{
		return vector3<T>{v.x * f, v.y * f, v.z * f};
	}

	// Typedefs
	//

	using vector3f = vector3<float>;
}

#endif // LANTERN_VECTOR3_H
