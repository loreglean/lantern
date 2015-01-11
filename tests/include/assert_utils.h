#ifndef LANTERN_ASSERT_UTILS_H
#define LANTERN_ASSERT_UTILS_H

#include <algorithm>
#include "gtest/gtest.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "color.h"
#include "texture.h"

using namespace lantern;

static const float tests_epsilon = 0.0001f;

inline void assert_floats_near(float const f1, float const f2)
{
	ASSERT_NEAR(f1, f2, tests_epsilon);
}

template<typename T>
inline void assert_vectors2_near(vector2<T> const& v1, vector2<T> const& v2)
{
	assert_floats_near(v1.x, v2.x);
	assert_floats_near(v1.y, v2.y);
}

inline void assert_vectors3_near(vector3 const& v1, vector3 const& v2)
{
	assert_floats_near(v1.x, v2.x);
	assert_floats_near(v1.y, v2.y);
	assert_floats_near(v1.z, v2.z);
}

inline void assert_vectors4_near(vector4 const& v1, vector4 const& v2)
{
	assert_floats_near(v1.x, v2.x);
	assert_floats_near(v1.y, v2.y);
	assert_floats_near(v1.z, v2.z);
	assert_floats_near(v1.w, v2.w);
}

inline void assert_matrix3x3_near(matrix3x3 const& m1, matrix3x3 const& m2)
{
	assert_floats_near(m1.values[0][0], m2.values[0][0]);
	assert_floats_near(m1.values[0][1], m2.values[0][1]);
	assert_floats_near(m1.values[0][2], m2.values[0][2]);
	assert_floats_near(m1.values[1][0], m2.values[1][0]);
	assert_floats_near(m1.values[1][1], m2.values[1][1]);
	assert_floats_near(m1.values[1][2], m2.values[1][2]);
	assert_floats_near(m1.values[2][0], m2.values[2][0]);
	assert_floats_near(m1.values[2][1], m2.values[2][1]);
	assert_floats_near(m1.values[2][2], m2.values[2][2]);
}

inline void assert_matrix4x4_near(matrix4x4 const& m1, matrix4x4 const& m2)
{
	assert_floats_near(m1.values[0][0], m2.values[0][0]);
	assert_floats_near(m1.values[0][1], m2.values[0][1]);
	assert_floats_near(m1.values[0][2], m2.values[0][2]);
	assert_floats_near(m1.values[0][3], m2.values[0][3]);
	assert_floats_near(m1.values[1][0], m2.values[1][0]);
	assert_floats_near(m1.values[1][1], m2.values[1][1]);
	assert_floats_near(m1.values[1][2], m2.values[1][2]);
	assert_floats_near(m1.values[1][3], m2.values[1][3]);
	assert_floats_near(m1.values[2][0], m2.values[2][0]);
	assert_floats_near(m1.values[2][1], m2.values[2][1]);
	assert_floats_near(m1.values[2][2], m2.values[2][2]);
	assert_floats_near(m1.values[2][3], m2.values[2][3]);
	assert_floats_near(m1.values[3][0], m2.values[3][0]);
	assert_floats_near(m1.values[3][1], m2.values[3][1]);
	assert_floats_near(m1.values[3][2], m2.values[3][2]);
	assert_floats_near(m1.values[3][3], m2.values[3][3]);
}

inline void assert_pixel_color(texture const& texture, vector2ui const& point, color const& c)
{
	color const current_color = texture.get_pixel_color(point);
	bool const pixel_color_matches{current_color == c};
	ASSERT_TRUE(pixel_color_matches);
}

inline void assert_pixels_colors(
	texture const& texture,
	std::vector<vector2ui> const& points,
	color const& points_color)
{
	for (unsigned int i{0}; i < texture.get_width(); i++)
	{
		for (unsigned int j{0}; j < texture.get_height(); j++)
		{
			vector2ui p{i, j};
			if (std::find(std::begin(points), std::end(points), p) != std::end(points))
			{
				assert_pixel_color(texture, p, points_color);
			}
		}
	}
}

inline void assert_pixels_two_colors(
	texture const& texture,
	std::vector<vector2ui> const& points,
	color const& points_color, color const& other_pixels_color)
{
	for (unsigned int i{0}; i < texture.get_width(); i++)
	{
		for (unsigned int j{0}; j < texture.get_height(); j++)
		{
			vector2ui p{i, j};
			if (std::find(std::begin(points), std::end(points), p) != std::end(points))
			{
				assert_pixel_color(texture, p, points_color);
			}
			else
			{
				assert_pixel_color(texture, p, other_pixels_color);
			}
		}
	}
}

#endif // LANTERN_ASSERT_UTILS_H
