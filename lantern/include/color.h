#ifndef LANTERN_COLOR_H
#define LANTERN_COLOR_H

namespace lantern
{
	class color final
	{
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;

		bool operator==(color const& c) const;

		static const color BLACK;
		static const color WHITE;
		static const color RED;
		static const color GREEN;
		static const color BLUE;
	};
}

#endif // LANTERN_COLOR_H
