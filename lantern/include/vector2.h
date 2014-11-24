#ifndef LANTERN_VECTOR2_H
#define LANTERN_VECTOR2_H

#include <cmath>

namespace lantern
{
	template<typename T>
	class vector2 final
	{
	public:
		T x;
		T y;

		vector2();
		vector2(T const x, T const y);

		bool operator==(vector2<T> const& other) const;
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

	typedef vector2<float> vector2f;
	typedef vector2<unsigned int> vector2ui;
}

#endif
