#include "assert_utils.h"
#include "camera.h"

using namespace lantern;

TEST(camera, constructor)
{
	float cos_value{std::cos((float)M_PI/3.0f)};
	float sin_value{std::sin((float)M_PI/3.0f)};
	camera const c{vector3::ZERO, vector3{2 * cos_value, 0.0f, 2 * sin_value}, vector3::Y_UNIT, (float)M_PI / 2.0f, 0.5f, 0.01f, 100.0f};
	assert_floats_near(c.get_aspect_ratio(), 0.5f);
	assert_floats_near(c.get_horizontal_fov(), (float) M_PI / 2.0f);
	assert_floats_near(c.get_vertical_fov(), (float) M_PI / 4.0f);
	assert_vectors3_near(c.get_forward(), vector3{cos_value, 0.0f, sin_value});
	assert_vectors3_near(c.get_right(), vector3{sin_value, 0.0f, -cos_value});
	assert_vectors3_near(c.get_up(), vector3{0.0f, 1.0f, 0.0f});
	assert_vectors3_near(c.get_position(), vector3{0.0f, 0.0f, 0.0f});
}

TEST(camera, moving)
{
	float cos_value = std::cos((float)M_PI/3.0f);
	float sin_value = std::sin((float)M_PI/3.0f);
	camera c{vector3::ZERO, vector3{cos_value, 0.0f, sin_value}, vector3::Y_UNIT, (float)M_PI / 2.0f, 1.0f, 0.01f, 100.0f};

	c.move_forward(1.0f);
	assert_vectors3_near(c.get_position(), vector3{cos_value, 0.0f, sin_value});
	c.move_backward(2.0f);
	assert_vectors3_near(c.get_position(), vector3{-cos_value, 0.0f, -sin_value});

	c.set_position(vector3::ZERO);

	c.move_right(2.0f);
	assert_vectors3_near(c.get_position(), vector3{2 * sin_value, 0.0f, -2.0f * cos_value});
	c.move_left(5.0f);
	assert_vectors3_near(c.get_position(), vector3{-3.0f * sin_value, 0.0f, 3.0f * cos_value});

	c.move_up(7.0f);
	assert_vectors3_near(c.get_position(), vector3{-3.0f * sin_value, 7.0f, 3.0f * cos_value});
	c.move_down(4.5f);
	assert_vectors3_near(c.get_position(), vector3{-3.0f * sin_value, 2.5f, 3.0f * cos_value});
}

TEST(camera, rotation)
{
	camera c{vector3::ZERO, vector3::Z_UNIT, vector3::Y_UNIT, (float)M_PI / 2.0f, 1.0f, 0.01f, 100.0f};

	c.pitch((float)M_PI / 2.0f);
	assert_vectors3_near(c.get_forward(), vector3{0.0f, -1.0f, 0.0f});
	assert_vectors3_near(c.get_right(), vector3{1.0f, 0.0f, 0.0f});
	assert_vectors3_near(c.get_up(), vector3{0.0f, 0.0f, 1.0f});

	c.yaw((float)M_PI / 2.0f);
	assert_vectors3_near(c.get_forward(), vector3{0.0f, -1.0f, 0.0f});
	assert_vectors3_near(c.get_right(), vector3{0.0f, 0.0f, -1.0f});
	assert_vectors3_near(c.get_up(), vector3{1.0f, 0.0f, 0.0f});
}
