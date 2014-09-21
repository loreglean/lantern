#include <gtest/gtest.h>
#include <algorithm>
#include "bitmap_painter.h"

using namespace lantern;

// Helper function to ASSERT_TRUE that pixel color matches
//
static void assert_pixel_color(bitmap_painter const& painter, point2D const& point, color const& c)
{
	bool pixel_color_matches = (painter.get_pixel_color(point) == c);
	ASSERT_TRUE(pixel_color_matches);
}

// Helper function to ASSERT_TRUE that each pixel from "points" has color "points_color",
// all other pixels should have color "other_pixels_color"
//
static void assert_pixels_colors(
	bitmap_painter const& painter,
	std::vector<point2D> const& points, color const& points_color,
	color const& other_pixels_color)
{
	for (unsigned int i{0}; i < painter.get_bitmap_width(); i++)
	{
		for (unsigned int j{0}; j < painter.get_bitmap_height(); j++)
		{
			point2D p{i, j};

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
		assert_pixel_color(painter, point2D{i, y}, c);
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
	painter.draw_pixel(point2D{2, 2}, color::WHITE);
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
	assert_pixel_color(painter, point2D{2, 2}, color::WHITE);
}

TEST(bitmap_painter, draw_line)
{
	bitmap_painter painter{6, 6};

	// Horizontal
	//

	painter.clear(0);
	painter.draw_line(point2D{0, 0}, point2D{5, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{1, 0}, point2D{2, 0}, point2D{3, 0}, point2D{4, 0}, point2D{5, 0}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(point2D{5, 0}, point2D{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{1, 0}, point2D{2, 0}, point2D{3, 0}, point2D{4, 0}, point2D{5, 0}},
		color::BLUE,
		color::BLACK);

	// Vertical
	//

	painter.clear(0);
	painter.draw_line(point2D{0, 0}, point2D{0, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{0, 1}, point2D{0, 2}, point2D{0, 3}, point2D{0, 4}, point2D{0, 5}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(point2D{0, 5}, point2D{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{0, 1}, point2D{0, 2}, point2D{0, 3}, point2D{0, 4}, point2D{0, 5}},
		color::BLUE,
		color::BLACK);

	// Single point
	//

	painter.clear(0);
	painter.draw_line(point2D{0, 0}, point2D{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}},
		color::BLUE,
		color::BLACK);

	// Abs(slope) = 1
	//

	painter.clear(0);
	painter.draw_line(point2D{0, 0}, point2D{5, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{1, 1}, point2D{2, 2}, point2D{3, 3}, point2D{4, 4}, point2D{5, 5}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(point2D{5, 5}, point2D{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{1, 1}, point2D{2, 2}, point2D{3, 3}, point2D{4, 4}, point2D{5, 5}},
		color::BLUE,
		color::BLACK);

	// Abs(slope_x) < 1, abs(slope_y) > 1
	//

	painter.clear(0);
	painter.draw_line(point2D{0, 5}, point2D{2, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 5}, point2D{0, 4}, point2D{1, 3}, point2D{1, 2}, point2D{2, 1}, point2D{2, 0}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(point2D{2, 0}, point2D{0, 5}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 5}, point2D{0, 4}, point2D{1, 3}, point2D{1, 2}, point2D{2, 1}, point2D{2, 0}},
		color::BLUE,
		color::BLACK);

	// Abs(slope_x) > 1, abs(slope_y) < 1
	//

	painter.clear(0);
	painter.draw_line(point2D{0, 0}, point2D{5, 1}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{1, 0}, point2D{2, 0}, point2D{3, 1}, point2D{4, 1}, point2D{5, 1}},
		color::BLUE,
		color::BLACK);

	painter.clear(0);
	painter.draw_line(point2D{5, 1}, point2D{0, 0}, color::BLUE);
	assert_pixels_colors(
		painter,
		std::vector<point2D>{point2D{0, 0}, point2D{1, 0}, point2D{2, 0}, point2D{3, 1}, point2D{4, 1}, point2D{5, 1}},
		color::BLUE,
		color::BLACK);
}
