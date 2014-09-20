#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "camera.h"

using namespace lantern;

TEST(camera, constructor)
{
	float cos_value = std::cos((float)M_PI/3.0f);
	float sin_value = std::sin((float)M_PI/3.0f);
	camera const c{vector3::ZERO, vector3{2 * cos_value, 0.0f, 2 * sin_value}};
	ASSERT_FLOATS_NEAR(c.get_forward().x, cos_value);
	ASSERT_FLOATS_NEAR(c.get_forward().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_forward().z, sin_value);
	ASSERT_FLOATS_NEAR(c.get_right().x, sin_value);
	ASSERT_FLOATS_NEAR(c.get_right().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_right().z, -cos_value);
	ASSERT_FLOATS_NEAR(c.get_up().x, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_up().y, 1.0f);
	ASSERT_FLOATS_NEAR(c.get_up().z, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_position().x, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_position().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_position().z, 0.0f);
}

TEST(camera, setting_position)
{
	camera c{vector3::ZERO, vector3::Z_UNIT};
	c.set_position(vector3{0.12f, 0.1f, -2.0f});
	ASSERT_FLOATS_NEAR(c.get_position().x, 0.12f);
	ASSERT_FLOATS_NEAR(c.get_position().y, 0.1f);
	ASSERT_FLOATS_NEAR(c.get_position().z, -2.0f);
}

TEST(camera, moving)
{
	float cos_value = std::cos((float)M_PI/3.0f);
	float sin_value = std::sin((float)M_PI/3.0f);
	camera c{vector3::ZERO, vector3{cos_value, 0.0f, sin_value}};

	c.move_forward(1.0f);
	ASSERT_FLOATS_NEAR(c.get_position().x, cos_value);
	ASSERT_FLOATS_NEAR(c.get_position().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_position().z, sin_value);
	c.move_backward(2.0f);
	ASSERT_FLOATS_NEAR(c.get_position().x, -cos_value);
	ASSERT_FLOATS_NEAR(c.get_position().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_position().z, -sin_value);

	c.set_position(vector3::ZERO);

	c.move_right(2.0f);
	ASSERT_FLOATS_NEAR(c.get_position().x, 2 * sin_value);
	ASSERT_FLOATS_NEAR(c.get_position().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_position().z, 2 * (-cos_value));
	c.move_left(5.0f);
	ASSERT_FLOATS_NEAR(c.get_position().x, -3 * sin_value);
	ASSERT_FLOATS_NEAR(c.get_position().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_position().z, 3 * cos_value);

	c.move_up(7.0f);
	ASSERT_FLOATS_NEAR(c.get_position().x, -3 * sin_value);
	ASSERT_FLOATS_NEAR(c.get_position().y, 7.0f);
	ASSERT_FLOATS_NEAR(c.get_position().z, 3 * cos_value);
	c.move_down(4.5f);
	ASSERT_FLOATS_NEAR(c.get_position().x, -3 * sin_value);
	ASSERT_FLOATS_NEAR(c.get_position().y, 2.5f);
	ASSERT_FLOATS_NEAR(c.get_position().z, 3 * cos_value);
}

TEST(camera, rotation)
{
	camera c{vector3::ZERO, vector3::Z_UNIT};

	c.pitch((float)M_PI / 2.0f);
	ASSERT_FLOATS_NEAR(c.get_forward().x, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_forward().y, -1.0f);
	ASSERT_FLOATS_NEAR(c.get_forward().z, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_right().x, 1.0f);
	ASSERT_FLOATS_NEAR(c.get_right().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_right().z, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_up().x, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_up().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_up().z, 1.0f);

	c.yaw((float)M_PI / 2.0f);
	ASSERT_FLOATS_NEAR(c.get_forward().x, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_forward().y, -1.0f);
	ASSERT_FLOATS_NEAR(c.get_forward().z, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_right().x, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_right().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_right().z, -1.0f);
	ASSERT_FLOATS_NEAR(c.get_up().x, 1.0f);
	ASSERT_FLOATS_NEAR(c.get_up().y, 0.0f);
	ASSERT_FLOATS_NEAR(c.get_up().z, 0.0f);
}
