#include "common.h"
#include "matrix4x4.h"

using namespace lantern;

TEST(matrix4x4, constructors)
{
	matrix4x4 const m1{
		15.0f, -3.0f, 20.0f, 1.0f,
		13.1f, -1.1f, 17.1f, -0.15f,
		90.2f, 1.2f, -2.2f, 0.0f,
		0.0f, 2.0f, 1.0f, -3.0f};
	ASSERT_FLOATS_NEAR(m1.values[0][0], 15.0f);
	ASSERT_FLOATS_NEAR(m1.values[0][1], -3.0f);
	ASSERT_FLOATS_NEAR(m1.values[0][2], 20.0f);
	ASSERT_FLOATS_NEAR(m1.values[0][3], 1.0f);
	ASSERT_FLOATS_NEAR(m1.values[1][0], 13.1f);
	ASSERT_FLOATS_NEAR(m1.values[1][1], -1.1f);
	ASSERT_FLOATS_NEAR(m1.values[1][2], 17.1f);
	ASSERT_FLOATS_NEAR(m1.values[1][3], -0.15f);
	ASSERT_FLOATS_NEAR(m1.values[2][0], 90.2f);
	ASSERT_FLOATS_NEAR(m1.values[2][1], 1.2f);
	ASSERT_FLOATS_NEAR(m1.values[2][2], -2.2f);
	ASSERT_FLOATS_NEAR(m1.values[2][3], 0.0f);
	ASSERT_FLOATS_NEAR(m1.values[3][0], 0.0f);
	ASSERT_FLOATS_NEAR(m1.values[3][1], 2.0f);
	ASSERT_FLOATS_NEAR(m1.values[3][2], 1.0f);
	ASSERT_FLOATS_NEAR(m1.values[3][3], -3.0f);
}

TEST(matrix4x4, matrix_matrix_multiplication)
{
	matrix4x4 const m1{
		2.0f, -3.0f, 11.0f, 4.0f,
		-1.0f, -1.0f, 0.15f, 3.0f,
		1.0f, 0.77f, 0.33f, 0.25f,
		0.3f, -1.0f, -1.0f, -1.0f};
	matrix4x4 const m2{
		1.0f, 0.0f, 0.0f, -2.0f,
		-1.0f, 0.0f, 0.25f, 3.0f,
		1.0f, 0.13f, 0.99f, 2.0f,
		7.0f, 2.0f, -1.0f, 1.0f};
	matrix4x4 const m_muled{m1 * m2};
	ASSERT_FLOATS_NEAR(m_muled.values[0][0], 44.0f);
	ASSERT_FLOATS_NEAR(m_muled.values[0][1], 9.43f);
	ASSERT_FLOATS_NEAR(m_muled.values[0][2], 6.14f);
	ASSERT_FLOATS_NEAR(m_muled.values[0][3], 13.0f);
	ASSERT_FLOATS_NEAR(m_muled.values[1][0], 21.15f);
	ASSERT_FLOATS_NEAR(m_muled.values[1][1], 6.0195f);
	ASSERT_FLOATS_NEAR(m_muled.values[1][2], -3.1015f);
	ASSERT_FLOATS_NEAR(m_muled.values[1][3], 2.3f);
	ASSERT_FLOATS_NEAR(m_muled.values[2][0], 2.31f);
	ASSERT_FLOATS_NEAR(m_muled.values[2][1], 0.5429f);
	ASSERT_FLOATS_NEAR(m_muled.values[2][2], 0.2692f);
	ASSERT_FLOATS_NEAR(m_muled.values[2][3], 1.22f);
	ASSERT_FLOATS_NEAR(m_muled.values[3][0], -6.7f);
	ASSERT_FLOATS_NEAR(m_muled.values[3][1], -2.13f);
	ASSERT_FLOATS_NEAR(m_muled.values[3][2], -0.24f);
	ASSERT_FLOATS_NEAR(m_muled.values[3][3], -6.6f);
}

TEST(matrix4x4, vector_matrix_multiplication)
{
	vector4 const v{1.0f, 3.0f, -0.15f, 2.0f};
	matrix4x4 const m{
		1.0f, 0.0f, 0.0f, 0.1f,
		-1.0f, 0.0f, 0.25f, 3.0f,
		1.0f, 0.13f, 0.99f, 2.0f,
		-1.0f, 2.0f, 0.15f, 0.01f};
	vector4 const v_muled{v * m};
	ASSERT_FLOATS_NEAR(v_muled.x, -4.15f);
	ASSERT_FLOATS_NEAR(v_muled.y, 3.9805f);
	ASSERT_FLOATS_NEAR(v_muled.z, 0.9015f);
	ASSERT_FLOATS_NEAR(v_muled.w, 8.8200f);
}

TEST(matrix4x4, translation)
{
	matrix4x4 const translation{matrix4x4::translation(0.15f, -0.2f, 1.3f)};

	vector4 const v_point{1.0f, 2.0f, -3.0f, 1.0f};
	vector4 const v_point_translated{v_point * translation};
	ASSERT_FLOATS_NEAR(v_point_translated.x, 1.15f);
	ASSERT_FLOATS_NEAR(v_point_translated.y, 1.8f);
	ASSERT_FLOATS_NEAR(v_point_translated.z, -1.7f);
	ASSERT_FLOATS_NEAR(v_point_translated.w, 1.0f);

	vector4 const v_vector{1.0f, 2.0f, -3.0f, 0.0f};
	vector4 const v_vector_translated{v_vector * translation};
	ASSERT_FLOATS_NEAR(v_vector_translated.x, v_vector.x);
	ASSERT_FLOATS_NEAR(v_vector_translated.y, v_vector.y);
	ASSERT_FLOATS_NEAR(v_vector_translated.z, v_vector.z);
	ASSERT_FLOATS_NEAR(v_vector_translated.w, v_vector.w);
}

TEST(matrix4x4, scaling)
{
	vector4 const v{2.0f, 3.0f, -0.15f, 1.0f};

	matrix4x4 const scale{matrix4x4::scale(0.33f, 0.25f, 2.0f)};
	vector4 const v_scaled{v * scale};
	ASSERT_FLOATS_NEAR(v_scaled.x, 0.66f);
	ASSERT_FLOATS_NEAR(v_scaled.y, 0.75f);
	ASSERT_FLOATS_NEAR(v_scaled.z, -0.30f);
	ASSERT_FLOATS_NEAR(v_scaled.w, v.w);

	matrix4x4 const uniform_scale{matrix4x4::uniform_scale(5.0f)};
	vector4 const v_uniformly_scaled{v * uniform_scale};
	ASSERT_FLOATS_NEAR(v_uniformly_scaled.x, 10.0f);
	ASSERT_FLOATS_NEAR(v_uniformly_scaled.y, 15.0f);
	ASSERT_FLOATS_NEAR(v_uniformly_scaled.z, -0.75f);
	ASSERT_FLOATS_NEAR(v_uniformly_scaled.w, v.w);
}

TEST(matrix4x4, rotation)
{
	vector4 const v{0.5f, 1.0f, -1.0f, 1.0f};

	matrix4x4 const m_rotation_around_x_axis{matrix4x4::rotation_around_x_axis(M_PI / 3.0f)};
	vector4 const v_rotated_around_x_axis{v * m_rotation_around_x_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_x_axis.x, 0.5f);
	ASSERT_FLOATS_NEAR(v_rotated_around_x_axis.y, 1.366f);
	ASSERT_FLOATS_NEAR(v_rotated_around_x_axis.z, 0.366f);
	ASSERT_FLOATS_NEAR(v_rotated_around_x_axis.w, v.w);

	matrix4x4 const m_rotation_around_y_axis{matrix4x4::rotation_around_y_axis(M_PI / 10.0f)};
	vector4 const v_rotated_around_y_axis{v * m_rotation_around_y_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_y_axis.x, 0.1665f);
	ASSERT_FLOATS_NEAR(v_rotated_around_y_axis.y, 1.0f);
	ASSERT_FLOATS_NEAR(v_rotated_around_y_axis.z, -1.1055f);
	ASSERT_FLOATS_NEAR(v_rotated_around_y_axis.w, v.w);

	matrix4x4 const m_rotation_around_z_axis{matrix4x4::rotation_around_z_axis(M_PI / 4.0f)};
	vector4 const v_rotated_around_z_axis{v * m_rotation_around_z_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_z_axis.x, -0.3535f);
	ASSERT_FLOATS_NEAR(v_rotated_around_z_axis.y, 1.0606f);
	ASSERT_FLOATS_NEAR(v_rotated_around_z_axis.z, -1.0f);
	ASSERT_FLOATS_NEAR(v_rotated_around_z_axis.w, v.w);

	vector3 const axis{0.55f, -1.0f, -1.0f};
	matrix4x4 const m_rotation_around_axis{matrix4x4::rotation_around_axis(axis, M_PI / 2.0f)};
	vector4 const v_rotated_around_axis{v * m_rotation_around_axis};
	ASSERT_FLOATS_NEAR(v_rotated_around_axis.x, 1.3837f);
	ASSERT_FLOATS_NEAR(v_rotated_around_axis.y, -0.0864f);
	ASSERT_FLOATS_NEAR(v_rotated_around_axis.z, 0.5725f);
	ASSERT_FLOATS_NEAR(v_rotated_around_axis.w, v.w);
}
