#include "assert_utils.h"

using namespace lantern;

TEST(vector4, constructors)
{
	vector4 const v{2.0f, -4.0f, 15.0f, 1.0f};
	assert_floats_near(v.x, 2.0f);
	assert_floats_near(v.y, -4.0f);
	assert_floats_near(v.z, 15.0f);
	assert_floats_near(v.w, 1.0f);

	vector4 const v_default{};
	assert_floats_near(v_default.x, 0.0f);
	assert_floats_near(v_default.y, 0.0f);
	assert_floats_near(v_default.z, 0.0f);
	assert_floats_near(v_default.w, 0.0f);
}
