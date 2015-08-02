#ifndef LANTERN_COLOR_H
#define LANTERN_COLOR_H

#include "common_math.h"
namespace lantern
{
	/** Class representing RGB color */
	class color final
	{
	public:
		/** Red component */
		float r;

		/** Green component */
		float g;

		/** Blue component */
		float b;

		// Operators
		//

		bool operator==(color const& c) const;
		bool operator!=(color const& c) const;
		color operator*(float const s) const;
		color operator+(color const& c) const;

		static const color BLACK;
		static const color WHITE;
		static const color RED;
		static const color GREEN;
		static const color BLUE;
	};

	inline color color::operator*(float const s) const
	{
		return color{r * s, g * s, b * s};
	}

	inline color color::operator+(color const& c) const
	{
		return color{r + c.r, g + c.g, b + c.b};
	}

	inline bool color::operator==(color const& c) const
	{
		return (equals(this->r, c.r) && equals(this->g, c.g) && equals(this->b, c.b));
	}

	inline bool color::operator!=(color const& c) const
	{
		return !(*this == c);
	}
}

#endif // LANTERN_COLOR_H
