#ifndef LANTERN_BITMAP_PAINTER_H
#define LANTERN_BITMAP_PAINTER_H

#include <cstddef>
#include "point.h"
#include "color.h"

namespace lantern
{
	/** Class that implements drawing on a given size bitmap.
	*	It works in ARGB8888 data format.
	*/
	class bitmap_painter final
	{
	public:
		bitmap_painter(unsigned int const width, unsigned int const height);
		~bitmap_painter();

		unsigned int get_bitmap_width() const;
		unsigned int get_bitmap_height() const;
		int get_pitch() const;
		unsigned char const* get_data() const;
		color get_pixel_color(point2D const& point) const;

		// TODO: clear with custom color, not only gray
		void clear(unsigned char const bytes_value);
		void draw_pixel(point2D const &point, color const &c);
		void draw_line(point2D const& p0, point2D const& p1, color const& c);

	private:
		unsigned int m_bitmap_width;
		unsigned int m_bitmap_height;
		size_t m_data_total_size;
		unsigned char* m_data;
		unsigned int m_pitch;
	};
}

#endif // LANTERN_BITMAP_PAINTER_H
