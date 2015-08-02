#ifndef LANTERN_COLOR_H
#define LANTERN_COLOR_H

#include "math_common.h"
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
}

#endif // LANTERN_COLOR_H
