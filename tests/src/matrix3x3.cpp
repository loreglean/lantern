#include "common.h"
#include "matrix3x3.h"

using namespace lantern;

TEST(matrix3x3, constructors)
{
	matrix3x3 const m1{
		15.0f, -3.0f, 20.0f,
		13.1f, -1.1f, 17.1f,
		90.2f, 1.2f, -2.2f};
	assert_floats_near(m1.values[0][0], 15.0f);
	assert_floats_near(m1.values[0][1], -3.0f);
	assert_floats_near(m1.values[0][2], 20.0f);
	assert_floats_near(m1.values[1][0], 13.1f);
	assert_floats_near(m1.values[1][1], -1.1f);
	assert_floats_near(m1.values[1][2], 17.1f);
	assert_floats_near(m1.values[2][0], 90.2f);
	assert_floats_near(m1.values[2][1], 1.2f);
	assert_floats_near(m1.values[2][2], -2.2f);
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
	assert_floats_near(m_muled.values[0][0], 16.0f);
	assert_floats_near(m_muled.values[0][1], 1.43f);
	assert_floats_near(m_muled.values[0][2], 10.14f);
	assert_floats_near(m_muled.values[1][0], 0.15f);
	assert_floats_near(m_muled.values[1][1], 0.0195f);
	assert_floats_near(m_muled.values[1][2], -0.1015f);
	assert_floats_near(m_muled.values[2][0], 0.56f);
	assert_floats_near(m_muled.values[2][1], 0.0429f);
	assert_floats_near(m_muled.values[2][2], 0.5192);
}

TEST(matrix3x3, vector_matrix_multiplication)
{
	vector3 const v{1.0f, 3.0f, -0.15f};
	matrix3x3 const m{
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.25f,
		1.0f, 0.13f, 0.99f};
	vector3 const v_muled{v * m};
	assert_floats_near(v_muled.x, -2.15f);
	assert_floats_near(v_muled.y, -0.0195f);
	assert_floats_near(v_muled.z, 0.6015f);
}

TEST(matrix3x3, scaling)
{
	vector3 const v{2.0f, 3.0f, -0.15f};

	matrix3x3 const scale{matrix3x3::scale(0.33f, 0.25f, 2.0f)};
	vector3 const v_scaled{v * scale};
	assert_floats_near(v_scaled.x, 0.66f);
	assert_floats_near(v_scaled.y, 0.75f);
	assert_floats_near(v_scaled.z, -0.30f);

	matrix3x3 const uniform_scale{matrix3x3::uniform_scale(5.0f)};
	vector3 const v_uniformly_scaled{v * uniform_scale};
	assert_floats_near(v_uniformly_scaled.x, 10.0f);
	assert_floats_near(v_uniformly_scaled.y, 15.0f);
	assert_floats_near(v_uniformly_scaled.z, -0.75f);
}

TEST(matrix3x3, rotation)
{
	vector3 const v{0.5f, 1.0f, -1.0f};

	matrix3x3 const m_rotation_around_x_axis{matrix3x3::rotation_around_x_axis(M_PI / 3.0f)};
	vector3 const v_rotated_around_x_axis{v * m_rotation_around_x_axis};
	assert_floats_near(v_rotated_around_x_axis.x, 0.5f);
	assert_floats_near(v_rotated_around_x_axis.y, 1.366f);
	assert_floats_near(v_rotated_around_x_axis.z, 0.366f);

	matrix3x3 const m_rotation_around_y_axis{matrix3x3::rotation_around_y_axis(M_PI / 10.0f)};
	vector3 const v_rotated_around_y_axis{v * m_rotation_around_y_axis};
	assert_floats_near(v_rotated_around_y_axis.x, 0.1665f);
	assert_floats_near(v_rotated_around_y_axis.y, 1.0f);
	assert_floats_near(v_rotated_around_y_axis.z, -1.1055f);

	matrix3x3 const m_rotation_around_z_axis{matrix3x3::rotation_around_z_axis(M_PI / 4.0f)};
	vector3 const v_rotated_around_z_axis{v * m_rotation_around_z_axis};
	assert_floats_near(v_rotated_around_z_axis.x, -0.3535f);
	assert_floats_near(v_rotated_around_z_axis.y, 1.0606f);
	assert_floats_near(v_rotated_around_z_axis.z, -1.0f);

	vector3 const axis{0.55f, -1.0f, -1.0f};
	matrix3x3 const m_rotation_around_axis{matrix3x3::rotation_around_axis(axis, M_PI / 2.0f)};
	vector3 const v_rotated_around_axis{v * m_rotation_around_axis};
	assert_floats_near(v_rotated_around_axis.x, 1.3837f);
	assert_floats_near(v_rotated_around_axis.y, -0.0864f);
	assert_floats_near(v_rotated_around_axis.z, 0.5725f);
}
