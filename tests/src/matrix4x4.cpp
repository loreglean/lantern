#include "assert_utils.h"
#include "matrix4x4.h"

using namespace lantern;

TEST(matrix4x4, constructors)
{
	matrix4x4 const m1{
		15.0f, -3.0f, 20.0f, 1.0f,
		13.1f, -1.1f, 17.1f, -0.15f,
		90.2f, 1.2f, -2.2f, 0.0f,
		0.0f, 2.0f, 1.0f, -3.0f};
	assert_floats_near(m1.values[0][0], 15.0f);
	assert_floats_near(m1.values[0][1], -3.0f);
	assert_floats_near(m1.values[0][2], 20.0f);
	assert_floats_near(m1.values[0][3], 1.0f);
	assert_floats_near(m1.values[1][0], 13.1f);
	assert_floats_near(m1.values[1][1], -1.1f);
	assert_floats_near(m1.values[1][2], 17.1f);
	assert_floats_near(m1.values[1][3], -0.15f);
	assert_floats_near(m1.values[2][0], 90.2f);
	assert_floats_near(m1.values[2][1], 1.2f);
	assert_floats_near(m1.values[2][2], -2.2f);
	assert_floats_near(m1.values[2][3], 0.0f);
	assert_floats_near(m1.values[3][0], 0.0f);
	assert_floats_near(m1.values[3][1], 2.0f);
	assert_floats_near(m1.values[3][2], 1.0f);
	assert_floats_near(m1.values[3][3], -3.0f);
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
	assert_matrix4x4_near(
		m_muled,
		matrix4x4{
			44.0f, 9.43f, 6.14f, 13.0f,
			21.15f, 6.0195f, -3.1015f, 2.3f,
			2.31f, 0.5429f, 0.2692f, 1.22f,
			-6.7f, -2.13f, -0.24f, -6.6f});
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
	assert_vectors4_near(v_muled, vector4{-4.15f, 3.9805f, 0.9015f, 8.8200f});
}

TEST(matrix4x4, translation)
{
	matrix4x4 const translation{matrix4x4::translation(0.15f, -0.2f, 1.3f)};

	vector4 const v_point{1.0f, 2.0f, -3.0f, 1.0f};
	vector4 const v_point_translated{v_point * translation};
	assert_vectors4_near(v_point_translated, vector4{1.15f, 1.8f, -1.7f, 1.0f});

	vector4 const v_vector{1.0f, 2.0f, -3.0f, 0.0f};
	vector4 const v_vector_translated{v_vector * translation};
	assert_vectors4_near(v_vector_translated, v_vector);
}

TEST(matrix4x4, scaling)
{
	vector4 const v{2.0f, 3.0f, -0.15f, 1.0f};

	matrix4x4 const scale{matrix4x4::scale(0.33f, 0.25f, 2.0f)};
	vector4 const v_scaled{v * scale};
	assert_vectors4_near(v_scaled, vector4{0.66f, 0.75f, -0.30f, v.w});

	matrix4x4 const uniform_scale{matrix4x4::uniform_scale(5.0f)};
	vector4 const v_uniformly_scaled{v * uniform_scale};
	assert_vectors4_near(v_uniformly_scaled, vector4{10.0f, 15.0f, -0.75f, v.w});
}

TEST(matrix4x4, rotation)
{
	vector4 const v{0.5f, 1.0f, -1.0f, 1.0f};

	matrix4x4 const m_rotation_around_x_axis{matrix4x4::rotation_around_x_axis(static_cast<float>(M_PI) / 3.0f)};
	vector4 const v_rotated_around_x_axis{v * m_rotation_around_x_axis};
	assert_vectors4_near(v_rotated_around_x_axis, vector4{0.5f, 1.366f, 0.366f, v.w});

	matrix4x4 const m_rotation_around_y_axis{matrix4x4::rotation_around_y_axis(static_cast<float>(M_PI) / 10.0f)};
	vector4 const v_rotated_around_y_axis{v * m_rotation_around_y_axis};
	assert_vectors4_near(v_rotated_around_y_axis, vector4{0.1665f, 1.0f, -1.1055f, v.w});

	matrix4x4 const m_rotation_around_z_axis{matrix4x4::rotation_around_z_axis(static_cast<float>(M_PI) / 4.0f)};
	vector4 const v_rotated_around_z_axis{v * m_rotation_around_z_axis};
	assert_vectors4_near(v_rotated_around_z_axis, vector4{-0.3535f, 1.0606f, -1.0f, v.w});

	vector3f const axis{0.55f, -1.0f, -1.0f};
	matrix4x4 const m_rotation_around_axis{matrix4x4::rotation_around_axis(axis, static_cast<float>(M_PI) / 2.0f)};
	vector4 const v_rotated_around_axis{v * m_rotation_around_axis};
	assert_vectors4_near(v_rotated_around_axis, vector4{1.3837f, -0.0864f, 0.5725f, v.w});
}
