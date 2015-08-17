#ifndef LANTERN_VECTOR2_H
#define LANTERN_VECTOR2_H

#include "math_common.h"
#include "vector3.h"
#include "vector2.h"

namespace lantern
{
	/** 2-dimensional vector template class */
	template<typename T>
	class vector2 final
	{
	public:
		/** X-coordinate of vector */
		T x;

		/** Y-coordinate of vector */
		T y;

		/** Default constructor (initializes with default values) */
		vector2();

		/** Constructs vector with specified values
		* @param x X-coordinate of a vector
		* @param y Y-coordinate of a vector
		*/
		vector2(T const x, T const y);

		// Operators
		//

		bool operator==(vector2<T> const& v) const;
		vector2<T> operator+(vector2<T> const& v) const;
		vector2<T>& operator+=(vector2<T> const& v);
		vector2<T> operator-(vector2<T> const& v) const;
		vector2<T>& operator-=(vector2<T> const& v);
		vector2<T> operator-() const;
		vector2<T> operator*(float const f) const;
		vector2<T>& operator*=(float const f);
		vector2<T> operator/(float const f) const;
		vector2<T>& operator/=(float const f);

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
		vector2<T> normalized() const;

		/** Calculates dot product with v vector
		* @returns Dot product
		*/
		float dot(vector2<T> const& v) const;

		/* Calculates distance from one point to v
		* @param p v point
		* @returns Distance between two points */
		float distance_to(vector2<T> const& v) const;

		/** Calculates angle with v vector
		* @returns Angle in radians
		*/
		float angle_with(vector2<T> const& v) const;

		/** Calculates projection of this vector on v vector
		* @returns Projection vector
		*/
		vector2<T> projection_on(vector2<T> const& v) const;

		/** Calculates perpendicular from this vector to v vector
		* @returns Perpendicular
		*/
		vector2<T> perpendicular_on(vector2 const& v) const;
	};

	// vector2<T> implementation
	//

	template<typename T>
	inline vector2<T>::vector2()
		: x{}, y{}
	{
	}

	template<typename T>
	inline vector2<T>::vector2(T const x, T const y)
		: x{x}, y{y}
	{

	}

	template<typename T>
	inline bool vector2<T>::operator==(vector2<T> const& v) const
	{
		return equals(this->x, v.x) && equals(this->y, v.y);
	}

	template<typename T>
	inline vector2<T> vector2<T>::operator+(vector2<T> const& v) const
	{
		return vector2<T>{this->x + v.x, this->y + v.y};
	}

	template<typename T>
	inline vector2<T>& vector2<T>::operator+=(vector2<T> const& v)
	{
		this->x += v.x;
		this->y += v.y;

		return *this;
	}

	template<typename T>
	inline vector2<T> vector2<T>::operator-(vector2<T> const& v) const
	{
		return vector2{this->x - v.x, this->y - v.y};
	}

	template<typename T>
	inline vector2<T>& vector2<T>::operator-=(vector2<T> const& v)
	{
		this->x -= v.x;
		this->y -= v.y;

		return *this;
	}

	template<typename T>
	inline vector2<T> vector2<T>::operator-() const
	{
		return vector2<T>{-this->x, -this->y};
	}

	template<typename T>
	inline vector2<T> vector2<T>::operator*(float const f) const
	{
		return vector2<T>{this->x * f, this->y * f};
	}

	template<typename T>
	inline vector2<T>& vector2<T>::operator*=(float const f)
	{
		this->x *= f;
		this->y *= f;

		return *this;
	}

	template<typename T>
	inline vector2<T> vector2<T>::operator/(float const f) const
	{
		return vector2{this->x / f, this->y / f};
	}

	template<typename T>
	inline vector2<T>& vector2<T>::operator/=(float const f)
	{
		this->x /= f;
		this->y /= f;

		return *this;
	}

	template<typename T>
	inline float vector2<T>::length() const
	{
		return std::sqrt(this->x * this->x + this->y * this->y);
	}

	template<typename T>
	inline float vector2<T>::length_sqr() const
	{
		return this->x * this->x + this->y * this->y;
	}

	template<typename T>
	inline void vector2<T>::normalize()
	{
		float const length_reciprocal{1.0f / this->length()};

		x *= length_reciprocal;
		y *= length_reciprocal;
	}

	template<typename T>
	inline vector2<T> vector2<T>::normalized() const
	{
		float const length_reciprocal{1.0f / this->length()};

		return vector3<T>{this->x * length_reciprocal, this->y * length_reciprocal};
	}

	template<typename T>
	inline float vector2<T>::dot(vector2<T> const& v) const
	{
		return this->x * v.x + this->y * v.y;
	}

	template<typename T>
	inline float vector2<T>::distance_to(vector2<T> const& v) const
	{
		float const dist_x{v.x - this->x};
		float const dist_y{v.y - this->y};

		return std::sqrt(dist_x * dist_x + dist_y * dist_y);
	}

	template<typename T>
	inline float vector2<T>::angle_with(vector2<T> const& v) const
	{
		return std::acos(this->dot(v) / (this->length() * v.length()));
	}

	template<typename T>
	inline vector2<T> vector2<T>::projection_on(vector2<T> const& v) const
	{
		return v.normalized() * (this->dot(v) / v.length());
	}

	template<typename T>
	inline vector2<T> vector2<T>::perpendicular_on(vector2<T> const& v) const
	{
		return (*this) - this->projection_on(v);
	}

	// Typedefs
	//

	using vector2f = vector2<float>;
	using vector2ui = vector2<unsigned int>;
}

#endif
