#ifndef LANTERN_COLOR_H
#define LANTERN_COLOR_H

namespace lantern
{
	/** Class representing RGB color */
	class color final
	{
	public:
		/** Red component */
		unsigned char r;

		/** Green component */
		unsigned char g;

		/** Blue component */
		unsigned char b;

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
		return color{
			static_cast<unsigned char>(r * s),
			static_cast<unsigned char>(g * s),
			static_cast<unsigned char>(b * s)};
	}

	inline color color::operator+(color const& c) const
	{
		return color{
			static_cast<unsigned char>(r + c.r),
			static_cast<unsigned char>(g + c.g),
			static_cast<unsigned char>(b + c.b)};
	}

	inline bool color::operator==(color const& c) const
	{
		return ((r == c.r) && (g == c.g) && (b == c.b));
	}

	inline bool color::operator!=(color const& c) const
	{
		return !(*this == c);
	}
}

#endif // LANTERN_COLOR_H
