#ifndef LANTERN_VECTOR2_H
#define LANTERN_VECTOR2_H

#include <cmath>

namespace lantern
{
	/** Simple 2-dimensional vector */
	template<typename T>
	class vector2 final
	{
	public:
		T x;
		T y;

		vector2();
		vector2(T const x, T const y);

		bool operator==(vector2<T> const& other) const;
		vector2 operator*(float const s) const;
		vector2 operator+(vector2 const& v) const;
	};

	template<typename T>
	vector2<T>::vector2()
		: x{}, y{}
	{
	}

	template<typename T>
	vector2<T>::vector2(T const x, T const y)
		: x{x}, y{y}
	{

	}

	template<typename T>
	bool vector2<T>::operator==(vector2<T> const& other) const
	{
		return ((x == other.x) && (y == other.y));
	}

	template<typename T>
	vector2<T> vector2<T>::operator*(float const s) const
	{
		return vector2<T>{x * s, y * s};
	}

	template<typename T>
	vector2<T> vector2<T>::operator+(vector2<T> const& v) const
{
	return vector2<T>{x + v.x, y + v.y};
}

	typedef vector2<float> vector2f;
	typedef vector2<unsigned int> vector2ui;
}

#endif
