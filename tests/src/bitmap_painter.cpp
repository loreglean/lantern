#include <algorithm>
#include "common.h"
#include "bitmap_painter.h"

using namespace lantern;

// Helper function to ASSERT_TRUE that pixel color matches
//
static void assert_pixel_color(bitmap_painter const& painter, vector2ui const& point, color const& c)
{
	color const current_color = painter.get_pixel_color(point);
	bool const pixel_color_matches{current_color == c};

	if (!pixel_color_matches)
	{
		int i = 0;
		(void)i;
	}

	ASSERT_TRUE(pixel_color_matches);
}

// Helper function to ASSERT_TRUE that each pixel from "points" has color "points_color",
// all other pixels should have color "other_pixels_color"
//
static void assert_pixels_colors(
	bitmap_painter const& painter,
	std::vector<vector2ui> const& points, color const& points_color,
	color const& other_pixels_color)
{
	for (unsigned int i{0}; i < painter.get_bitmap_width(); i++)
	{
		for (unsigned int j{0}; j < painter.get_bitmap_height(); j++)
		{
			vector2ui p{i, j};

			if (std::find(std::begin(points), std::end(points), p) != std::end(points))
			{
				assert_pixel_color(painter, p, points_color);
			}
			else
			{
				assert_pixel_color(painter, p, other_pixels_color);
			}
		}
	}
}

// Helper function to ASSERT_TRUE that each pixel in a horizontal line has a required color
//
static void assert_horizontal_line_color(
	bitmap_painter const& painter,
	unsigned int const x0, unsigned int const x1, unsigned int const y,
	color const& c)
{
	for (unsigned int i{x0}; i <= x1; i++)
	{
		assert_pixel_color(painter, vector2ui{i, y}, c);
	}
}

TEST(bitmap_painter, clear)
{
	bitmap_painter painter{6, 6};
	painter.clear(125);
	for (unsigned int i{0}; i < 6; i++)
	{
		assert_horizontal_line_color(painter, 0, 5, i, color{125, 125, 125});
	}
}

TEST(bitmap_painter, draw_pixel)
{
	bitmap_painter painter{6, 6};
	painter.clear(0);
	painter.draw_pixel(vector2ui{2, 2}, color::WHITE);
	for (unsigned int i{0}; i < 6; i++)
	{
		if (i != 2)
		{
			assert_horizontal_line_color(painter, 0, 5, i, color::BLACK);
		}
		else
		{
			assert_horizontal_line_color(painter, 0, 1, i, color::BLACK);
			assert_horizontal_line_color(painter, 3, 5, i, color::BLACK);
		}
	}
	assert_pixel_color(painter, vector2ui{2, 2}, color::WHITE);
}

TEST(bitmap_painter, draw_line)
{
	bitmap_painter painter{6, 6};

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


	// Point
	//

	painter.clear(0);
	painter.draw_line(vector2ui{1, 1}, vector2ui{1, 1}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{1, 1}},
		color::BLUE,
		color::BLACK);

	// Horizontal line
	//

	painter.clear(0);
	painter.draw_line(vector2ui{0, 3}, vector2ui{5, 3}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 3}, vector2ui{1, 3}, vector2ui{2, 3}, vector2ui{3, 3}, vector2ui{4, 3}, vector2ui{5, 3}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(vector2ui{5, 3}, vector2ui{0, 3}, color::BLUE);
	assert_pixels_colors(
			painter,
			std::vector<vector2ui>{vector2ui{0, 3}, vector2ui{1, 3}, vector2ui{2, 3}, vector2ui{3, 3}, vector2ui{4, 3}, vector2ui{5, 3}},
			color::BLUE,
			color::BLACK);

	// Vertical line
	//

	painter.clear(0);
	painter.draw_line(vector2ui{3, 0}, vector2ui{3, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{3, 0}, vector2ui{3, 1}, vector2ui{3, 2}, vector2ui{3, 3}, vector2ui{3, 4}, vector2ui{3, 5}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(vector2ui{3, 5}, vector2ui{3, 0}, color::BLUE);
	assert_pixels_colors(
			painter,
			std::vector<vector2ui>{vector2ui{3, 0}, vector2ui{3, 1}, vector2ui{3, 2}, vector2ui{3, 3}, vector2ui{3, 4}, vector2ui{3, 5}},
			color::BLUE,
			color::BLACK);

	// |Slope| = 1
	//

	painter.clear(0);
	painter.draw_line(vector2ui{0, 0}, vector2ui{5, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 0}, vector2ui{1, 1}, vector2ui{2, 2}, vector2ui{3, 3}, vector2ui{4, 4}, vector2ui{5, 5}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(vector2ui{5, 5}, vector2ui{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 0}, vector2ui{1, 1}, vector2ui{2, 2}, vector2ui{3, 3}, vector2ui{4, 4}, vector2ui{5, 5}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(vector2ui{0, 5}, vector2ui{5, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 5}, vector2ui{1, 4}, vector2ui{2, 3}, vector2ui{3, 2}, vector2ui{4, 1}, vector2ui{5, 0}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(vector2ui{5, 0}, vector2ui{0, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 5}, vector2ui{1, 4}, vector2ui{2, 3}, vector2ui{3, 2}, vector2ui{4, 1}, vector2ui{5, 0}},
		color::BLUE,
		color::BLACK);

	// Octant 0
	//

	// Through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{0, 0}, vector2ui{5, 2}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 0}, vector2ui{1, 0}, vector2ui{2, 1}, vector2ui{3, 1}, vector2ui{4, 2}, vector2ui{5, 2}},
		color::BLUE,
		color::BLACK);

	// Not through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{1, 0}, vector2ui{5, 3}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{1, 0}, vector2ui{2, 1}, vector2ui{3, 1}, vector2ui{4, 2}, vector2ui{5, 3}},
		color::BLUE,
		color::BLACK);

	// Octant 4
	//

	// Through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{5, 2}, vector2ui{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 0}, vector2ui{1, 0}, vector2ui{2, 1}, vector2ui{3, 1}, vector2ui{4, 2}, vector2ui{5, 2}},
		color::BLUE,
		color::BLACK);

	// Not through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{5, 3}, vector2ui{1, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{1, 0}, vector2ui{2, 1}, vector2ui{3, 1}, vector2ui{4, 2}, vector2ui{5, 3}},
		color::BLUE,
		color::BLACK);

	// Octant 1
	//

	// Through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{0, 0}, vector2ui{2, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 0}, vector2ui{0, 1}, vector2ui{1, 2}, vector2ui{1, 3}, vector2ui{2, 4}, vector2ui{2, 5}},
		color::BLUE,
		color::BLACK);

	// Not through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{1, 1}, vector2ui{2, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{1, 1}, vector2ui{1, 2}, vector2ui{2, 3}, vector2ui{2, 4}, vector2ui{2, 5}},
		color::BLUE,
		color::BLACK);

	// Octant 5
	//

	// Through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{2, 5}, vector2ui{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 0}, vector2ui{0, 1}, vector2ui{1, 2}, vector2ui{1, 3}, vector2ui{2, 4}, vector2ui{2, 5}},
		color::BLUE,
		color::BLACK);

	// Not through origin
	//

	painter.clear(0);
	painter.draw_line(vector2ui{2, 5}, vector2ui{1, 1}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{1, 1}, vector2ui{1, 2}, vector2ui{2, 3}, vector2ui{2, 4}, vector2ui{2, 5}},
		color::BLUE,
		color::BLACK);

	// Octant 6
	//

	painter.clear(0);
	painter.draw_line(vector2ui{0, 5}, vector2ui{2, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 5}, vector2ui{0, 4}, vector2ui{1, 3}, vector2ui{1, 2}, vector2ui{2, 1}, vector2ui{2, 0}},
		color::BLUE,
		color::BLACK);

	// Octant 2
	//

	painter.clear(0);
	painter.draw_line(vector2ui{2, 0}, vector2ui{0, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 5}, vector2ui{0, 4}, vector2ui{1, 3}, vector2ui{1, 2}, vector2ui{2, 1}, vector2ui{2, 0}},
		color::BLUE,
		color::BLACK);

	// Octant 7
	//

	painter.clear(0);
	painter.draw_line(vector2ui{0, 2}, vector2ui{5, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 2}, vector2ui{1, 2}, vector2ui{2, 1}, vector2ui{3, 1}, vector2ui{4, 0}, vector2ui{5, 0}},
		color::BLUE,
		color::BLACK);

	// Octant 3
	//

	painter.clear(0);
	painter.draw_line(vector2ui{5, 0}, vector2ui{0, 2}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<vector2ui>{vector2ui{0, 2}, vector2ui{1, 2}, vector2ui{2, 1}, vector2ui{3, 1}, vector2ui{4, 0}, vector2ui{5, 0}},
		color::BLUE,
		color::BLACK);
}
