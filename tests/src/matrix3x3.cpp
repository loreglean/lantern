#include <gtest/gtest.h>
#include "common.h"
#include "matrix3x3.h"

using namespace lantern;

TEST(matrix3x3, constructors)
{
	matrix3x3 const m1{
		15.0f, -3.0f, 20.0f,
		13.1f, -1.1f, 17.1f,
		90.2f, 1.2f, -2.2f};
	ASSERT_FLOATS_NEAR(m1.values[0][0], 15.0f);
	ASSERT_FLOATS_NEAR(m1.values[0][1], -3.0f);
	ASSERT_FLOATS_NEAR(m1.values[0][2], 20.0f);
	ASSERT_FLOATS_NEAR(m1.values[1][0], 13.1f);
	ASSERT_FLOATS_NEAR(m1.values[1][1], -1.1f);
	ASSERT_FLOATS_NEAR(m1.values[1][2], 17.1f);
	ASSERT_FLOATS_NEAR(m1.values[2][0], 90.2f);
	ASSERT_FLOATS_NEAR(m1.values[2][1], 1.2f);
	ASSERT_FLOATS_NEAR(m1.values[2][2], -2.2f);

	matrix3x3 const m2{};
	ASSERT_FLOATS_NEAR(m2.values[0][0], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[0][1], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[0][2], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[1][0], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[1][1], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[1][2], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[2][0], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[2][1], 0.0f);
	ASSERT_FLOATS_NEAR(m2.values[2][2], 0.0f);
}

TEST(matrix3x3, matrix_matrix_multiplication)
{
	matrix3x3 const m1{
		2.0f, -3.0f, 11.0f,
		-1.0f, -1.0f, 0.15f,
		1.0f, 0.77f, 0.33f};
	matrix3x3 const m2{
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.25f,
		1.0f, 0.13f, 0.99f};
	matrix3x3 const m_muled{m1 * m2};
	ASSERT_FLOATS_NEAR(m_muled.values[0][0], 16.0f);
	ASSERT_FLOATS_NEAR(m_muled.values[0][1], 1.43f);
	ASSERT_FLOATS_NEAR(m_muled.values[0][2], 10.14f);
	ASSERT_FLOATS_NEAR(m_muled.values[1][0], 0.15f);
	ASSERT_FLOATS_NEAR(m_muled.values[1][1], 0.0195f);
	ASSERT_FLOATS_NEAR(m_muled.values[1][2], -0.1015f);
	ASSERT_FLOATS_NEAR(m_muled.values[2][0], 0.56f);
	ASSERT_FLOATS_NEAR(m_muled.values[2][1], 0.0429f);
	ASSERT_FLOATS_NEAR(m_muled.values[2][2], 0.5192);
}

TEST(matrix3x3, vector_matrix_multiplication)
{
	vector3 const v{1.0f, 3.0f, -0.15f};
	matrix3x3 const m{
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.25f,
		1.0f, 0.13f, 0.99f};
	vector3 const v_muled{v * m};
	ASSERT_FLOATS_NEAR(v_muled.x, -2.15f);
	ASSERT_FLOATS_NEAR(v_muled.y, -0.0195f);
	ASSERT_FLOATS_NEAR(v_muled.z, 0.6015f);
}

TEST(matrix3x3, scaling)
{
	vector3 const v{2.0f, 3.0f, -0.15f};

	matrix3x3 const scale{matrix3x3::scale(0.33f, 0.25f, 2.0f)};
	vector3 const v_scaled{v * scale};
	ASSERT_FLOATS_NEAR(v_scaled.x, 0.66f);
	ASSERT_FLOATS_NEAR(v_scaled.y, 0.75f);
	ASSERT_FLOATS_NEAR(v_scaled.z, -0.30f);

	matrix3x3 const uniform_scale{matrix3x3::uniform_scale(5.0f)};
	vector3 const v_uniformly_scaled{v * uniform_scale};
	ASSERT_FLOATS_NEAR(v_uniformly_scaled.x, 10.0f);
	ASSERT_FLOATS_NEAR(v_uniformly_scaled.y, 15.0f);
	ASSERT_FLOATS_NEAR(v_uniformly_scaled.z, -0.75f);
}

TEST(matrix3x3, rotation)
{
	vector3 const v{0.5f, 1.0f, -1.0f};

	matrix3x3 const m_rotation_around_x_axis{matrix3x3::rotation_around_x_axis(M_PI / 3.0f)};
	vector3 const v_rotated_around_x_axis{v * m_rotation_around_x_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_x_axis.x, 0.5f);
	ASSERT_FLOATS_NEAR(v_rotated_around_x_axis.y, 1.366f);
	ASSERT_FLOATS_NEAR(v_rotated_around_x_axis.z, 0.366f);

	matrix3x3 const m_rotation_around_y_axis{matrix3x3::rotation_around_y_axis(M_PI / 10.0f)};
	vector3 const v_rotated_around_y_axis{v * m_rotation_around_y_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_y_axis.x, 0.1665f);
	ASSERT_FLOATS_NEAR(v_rotated_around_y_axis.y, 1.0f);
	ASSERT_FLOATS_NEAR(v_rotated_around_y_axis.z, -1.1055f);

	matrix3x3 const m_rotation_around_z_axis{matrix3x3::rotation_around_z_axis(M_PI / 4.0f)};
	vector3 const v_rotated_around_z_axis{v * m_rotation_around_z_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_z_axis.x, -0.3535f);
	ASSERT_FLOATS_NEAR(v_rotated_around_z_axis.y, 1.0606f);
	ASSERT_FLOATS_NEAR(v_rotated_around_z_axis.z, -1.0f);

	vector3 const axis{0.55f, -1.0f, -1.0f};
	matrix3x3 const m_rotation_around_axis{matrix3x3::rotation_around_axis(axis, M_PI / 2.0f)};
	vector3 const v_rotated_around_axis{v * m_rotation_around_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_axis.x, 1.3837f);
	ASSERT_FLOATS_NEAR(v_rotated_around_axis.y, -0.0864f);
	ASSERT_FLOATS_NEAR(v_rotated_around_axis.z, 0.5725f);
}
