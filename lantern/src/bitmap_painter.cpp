#include "bitmap_painter.h"
#include <cstring>
#include <utility>

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

color bitmap_painter::get_pixel_color(vector2ui const& point) const
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

void bitmap_painter::draw_line(vector2ui const& p0, vector2ui const& p1, color const& c)
{
	// See "Derivation" at http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	//

	// Octants are:
	//
	//      \    |    /
	//       \   |   /
	//        \ 5|6 /
	//        4\ | /7
	// --------  p0  -------
	//        3/ | \0
	//        / 2|1 \
	//       /   |   \
	//      /    |    \

	if (p0 == p1)
	{
		draw_pixel(p0, c);
		return;
	}

	if (p0.x == p1.x)
	{
		// Vertical line
		//

		if (p0.y < p1.y)
		{
			for (unsigned int i{p0.y}; i <= p1.y; ++i)
			{
				draw_pixel(vector2ui{p0.x, i}, c);
			}
		}
		else
		{
			for (unsigned int i{p1.y}; i <= p0.y; ++i)
			{
				draw_pixel(vector2ui{p0.x, i}, c);
			}
		}

		return;
	}

	if (p0.y == p1.y)
	{
		// Horizontal line
		//

		if (p0.x < p1.x)
		{
			for (unsigned int i{p0.x}; i <= p1.x; ++i)
			{
				draw_pixel(vector2ui{i, p0.y}, c);
			}
		}
		else
		{
			for (unsigned int i{p1.x}; i <= p0.x; ++i)
			{
				draw_pixel(vector2ui{i, p0.y}, c);
			}
		}

		return;
	}

	// Line is y = slope * x + offset
	//

	int const length_x{static_cast<int>(p1.x) - static_cast<int>(p0.x)};
	int const length_y{static_cast<int>(p1.y) - static_cast<int>(p0.y)};
	float const slope{static_cast<float>(length_y) / static_cast<float>(length_x)};
	float const offset{p0.y - slope * p0.x};

	// Every step we have two candidate points to be drawn
	// Which one we choose depends of mid-point value (positive or negative)
	//

	int dx_positive_plane;
	int dy_positive_plane;
	int dx_negative_plane;
	int dy_negative_plane;

	// Every step either x or y has a guarantee to change (+1 or -1)
	// Which one depends of the octant
	//

	unsigned int counter;
	unsigned int counter_max;
	int counter_delta;

	if (p1.x > p0.x)
	{
		// The right half-plane
		//

		if ((slope > 0.0f) && (slope < 1.0f))
		{
			// Octant 0
			//

			dx_positive_plane = 1;
			dy_positive_plane = 0;

			dx_negative_plane = 1;
			dy_negative_plane = 1;

			counter = p0.x;
			counter_delta = 1;
			counter_max = p1.x;
		}
		else if (slope >= 1.0f)
		{
			// Octant 1
			//

			dx_positive_plane = 1;
			dy_positive_plane = 1;

			dx_negative_plane = 0;
			dy_negative_plane = 1;

			counter = p0.y;
			counter_delta = 1;
			counter_max = p1.y;
		}
		else if (slope < -1.0f)
		{
			// Octant 6
			//

			dx_positive_plane = 0;
			dy_positive_plane = -1;

			dx_negative_plane = 1;
			dy_negative_plane = -1;

			counter = p0.y;
			counter_delta = -1;
			counter_max = p1.y;
		}
		else // ie if ((slope > 0) && (slope > -1))
		{
			// Octant 7
			//

			dx_positive_plane = 1;
			dy_positive_plane = -1;

			dx_negative_plane = 1;
			dy_negative_plane = 0;

			counter = p0.x;
			counter_delta = 1;
			counter_max = p1.x;
		}
	}
	else
	{
		// The left one
		//

		if ((slope > 0.0f) && (slope < 1.0f))
		{
			// Octant 4
			//

			dx_positive_plane = -1;
			dy_positive_plane = -1;

			dx_negative_plane = -1;
			dy_negative_plane = 0;

			counter = p0.x;
			counter_delta = -1;
			counter_max = p1.x;
		}
		else if (slope >= 1.0f)
		{
			// Octant 5
			//

			dx_positive_plane = 0;
			dy_positive_plane = -1;

			dx_negative_plane = -1;
			dy_negative_plane = -1;

			counter = p0.y;
			counter_delta = -1;
			counter_max = p1.y;
		}
		else if (slope < -1.0f)
		{
			// Octant 2
			//

			dx_positive_plane = -1;
			dy_positive_plane = 1;

			dx_negative_plane = 0;
			dy_negative_plane = 1;

			counter = p0.y;
			counter_delta = 1;
			counter_max = p1.y;
		}
		else // ie if ((slope > 0) && (slope > -1))
		{
			// Octant 3
			//

			dx_positive_plane = -1;
			dy_positive_plane = 0;

			dx_negative_plane = -1;
			dy_negative_plane = 1;

			counter = p0.x;
			counter_delta = -1;
			counter_max = p1.x;
		}
	}

	vector2ui current_point{p0.x, p0.y};

	float const mid_dx{static_cast<float>(dx_positive_plane + dx_negative_plane) / 2.0f};
	float const mid_dy{static_cast<float>(dy_positive_plane + dy_negative_plane) / 2.0f};

	draw_pixel(current_point, c);

	for (; counter != counter_max; counter += counter_delta)
	{
		float const mid_value{(current_point.y + mid_dy) - slope * (current_point.x + mid_dx) - offset};

		if (mid_value >= 0.0f)
		{
			current_point.x += dx_positive_plane;
			current_point.y += dy_positive_plane;
		}
		else
		{
			current_point.x += dx_negative_plane;
			current_point.y += dy_negative_plane;
		}

		draw_pixel(current_point, c);
	}
}
