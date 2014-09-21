#include "bitmap_painter.h"
#include <cstring>

using namespace lantern;

bitmap_painter::bitmap_painter(unsigned int const width, unsigned int const height)
	: m_data_total_size{width * height * 4},
	  m_data{new unsigned char[m_data_total_size]},
	  m_pitch{width * 4}
{
}

bitmap_painter::~bitmap_painter()
{
	delete[] m_data;
	m_data = nullptr;
}

unsigned char const* bitmap_painter::get_data() const
{
	return m_data;
}

int bitmap_painter::get_pitch() const
{
	return m_pitch;
}

void bitmap_painter::clear(unsigned char const bytes_value)
{
	memset(m_data, bytes_value, m_data_total_size);
}

void bitmap_painter::draw_pixel(
	unsigned int const x, unsigned int const y,
	unsigned char const r, unsigned char const g, unsigned char const b)
{
	unsigned int const pixel_first_byte_index = m_pitch * y + x * 4;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	m_data[pixel_first_byte_index + 0] = b;
	m_data[pixel_first_byte_index + 1] = g;
	m_data[pixel_first_byte_index + 2] = r;
	m_data[pixel_first_byte_index + 3] = 255;
#else
	m_data[pixel_first_byte_index + 0] = 255;
	m_data[pixel_first_byte_index + 1] = r;
	m_data[pixel_first_byte_index + 2] = g;
	m_data[pixel_first_byte_index + 3] = b;
#endif
}
