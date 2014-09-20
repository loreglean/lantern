#include <stddef.h>

#ifndef LANTERN_PAINTER_H
#define LANTERN_PAINTER_H

namespace lantern
{
	/** Class that implements drawing on a given size bitmap.
	*	It works in ARGB8888 data format.
	*/
	class bitmap_painter final
	{
	public:
		bitmap_painter(unsigned int width, unsigned int height);
		~bitmap_painter();

		unsigned char const* get_data() const;
		int get_pitch() const;

		void clear(unsigned char bytes_value);
		void draw_pixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);

	private:
		size_t m_data_total_size;
		unsigned char* m_data;
		unsigned int m_pitch;
	};
}

#endif // LANTERN_PAINTER_H
