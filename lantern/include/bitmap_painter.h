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
		bitmap_painter(unsigned int const width, unsigned int const height);
		~bitmap_painter();

		unsigned char const* get_data() const;
		int get_pitch() const;

		void clear(unsigned char const bytes_value);
		void draw_pixel(
			unsigned int const x, unsigned int const y,
			unsigned char const r, unsigned char const g, unsigned char const b);

	private:
		size_t m_data_total_size;
		unsigned char* m_data;
		unsigned int m_pitch;
	};
}

#endif // LANTERN_PAINTER_H
