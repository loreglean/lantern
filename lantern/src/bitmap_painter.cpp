#include "bitmap_painter.h"
#include <cstring>
#include <cmath>

using namespace lantern;

bitmap_painter::bitmap_painter(unsigned int const width, unsigned int const height)
	: m_bitmap_width{width},
	  m_bitmap_height{height},
	  m_data_total_size{width * height * 4},
	  m_data{new unsigned char[m_data_total_size]},
	  m_pitch{width * 4}
{
}

bitmap_painter::~bitmap_painter()
{
	delete[] m_data;
	m_data = nullptr;
}

unsigned int bitmap_painter::get_bitmap_width() const
{
	return m_bitmap_width;
}

unsigned int bitmap_painter::get_bitmap_height() const
{
	return m_bitmap_height;
}

int bitmap_painter::get_pitch() const
{
	return m_pitch;
}

unsigned char const* bitmap_painter::get_data() const
{
	return m_data;
}

color bitmap_painter::get_pixel_color(point2D const &point) const
{
	unsigned int const pixel_first_byte_index{m_pitch * point.y + point.x * 4};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return color{
		m_data[pixel_first_byte_index + 2],
		m_data[pixel_first_byte_index + 1],
		m_data[pixel_first_byte_index + 0]};
#else
	return color{
		m_data[pixel_first_byte_index + 1],
		m_data[pixel_first_byte_index + 2],
		m_data[pixel_first_byte_index + 3]};

#endif
}

void bitmap_painter::clear(unsigned char const bytes_value)
{
	memset(m_data, bytes_value, m_data_total_size);
}

void bitmap_painter::draw_pixel(point2D const &point, color const &c)
{
	unsigned int const pixel_first_byte_index{m_pitch * point.y + point.x * 4};


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	// m_data[pixel_first_byte_index + 3] is alpha, we don't use it for now
	m_data[pixel_first_byte_index + 0] = c.b;
	m_data[pixel_first_byte_index + 1] = c.g;
	m_data[pixel_first_byte_index + 2] = c.r;
#else
	// m_data[pixel_first_byte_index + 0] is alpha, we don't use it for now
	m_data[pixel_first_byte_index + 1] = c.r;
	m_data[pixel_first_byte_index + 2] = c.g;
	m_data[pixel_first_byte_index + 3] = c.b;
#endif
}

void bitmap_painter::draw_line(point2D const& p0, point2D const& p1, color const& c)
{
	// TODO: get rid of floats

	// Signed length of line across x axis
	int const x_signed_length{static_cast<int>(p1.x) - static_cast<int>(p0.x)};
	// Signed length of line across y axis
	int const y_signed_length{static_cast<int>(p1.y) - static_cast<int>(p0.y)};

	// How many x's we get for each y
	float const slope_x{std::abs(static_cast<float>(x_signed_length) / static_cast<float>(y_signed_length))};
	// How many y's we get for each x
	float const slope_y{std::abs(static_cast<float>(y_signed_length) / static_cast<float>(x_signed_length))};

	// Directions in which we move each cycle iteration
	//
	int const x_delta{p0.x < p1.x ? 1 : -1};
	int const y_delta{p0.y < p1.y ? 1 : -1};

	float current_slope_x{0.0f};
	float current_slope_y{0.0f};
	unsigned int x{p0.x};
	unsigned int y{p0.y};

	// Move until we get to the end point
	//
	while (x != p1.x || y != p1.y)
	{
		draw_pixel(point2D{x, y}, c);

		current_slope_x += slope_x;
		current_slope_y += slope_y;

		// If slope is bigger then 0.5f, it means we are closer to the next pixel in this direction
		//

		if (current_slope_y > 0.5f)
		{
			y += y_delta;
			current_slope_y = 0.0f;
		}

		if (current_slope_x > 0.5f)
		{
			x += x_delta;
			current_slope_x = 0.0f;
		}
	}

	draw_pixel(point2D{x, y}, c);
}
