#ifndef LANTERN_BITMAP_PAINTER_H
#define LANTERN_BITMAP_PAINTER_H

#include <cstddef>
#include "vector2.h"
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
		color get_pixel_color(vector2ui const& point) const;

		// TODO: clear with custom color, not only gray
		void clear(unsigned char const bytes_value);
		void draw_pixel(vector2ui const& point, color const& c);
		void draw_line(vector2ui const& p0, vector2ui const& p1, color const& c);

	private:
		unsigned int m_bitmap_width;
		unsigned int m_bitmap_height;
		size_t m_data_total_size;
		unsigned char* m_data;
		unsigned int m_pitch;
	};

	// Inlined due to huge amount of invocations
	//
	inline void bitmap_painter::draw_pixel(vector2ui const& point, color const& c)
	{
		unsigned int const pixel_first_byte_index{m_pitch * point.y + point.x * 4};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		m_data[pixel_first_byte_index + 0] = c.b;
		m_data[pixel_first_byte_index + 1] = c.g;
		m_data[pixel_first_byte_index + 2] = c.r;
		// m_data[pixel_first_byte_index + 3] is alpha, we don't use it for now
#else
		// m_data[pixel_first_byte_index + 0] is alpha, we don't use it for now
		m_data[pixel_first_byte_index + 1] = c.r;
		m_data[pixel_first_byte_index + 2] = c.g;
		m_data[pixel_first_byte_index + 3] = c.b;
#endif
	}
}

#endif // LANTERN_BITMAP_PAINTER_H
