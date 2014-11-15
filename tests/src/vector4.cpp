#include "common.h"
#include "vector4.h"

using namespace lantern;

TEST(vector4, constructors)
{
	vector4 const v{2.0f, -4.0f, 15.0f, 1.0f};
	ASSERT_FLOATS_NEAR(v.x, 2.0f);
	ASSERT_FLOATS_NEAR(v.y, -4.0f);
	ASSERT_FLOATS_NEAR(v.z, 15.0f);
	ASSERT_FLOATS_NEAR(v.w, 1.0f);

	vector4 const v_default{};
	ASSERT_FLOATS_NEAR(v_default.x, 0.0f);
	ASSERT_FLOATS_NEAR(v_default.y, 0.0f);
	ASSERT_FLOATS_NEAR(v_default.z, 0.0f);
	ASSERT_FLOATS_NEAR(v_default.w, 0.0f);
}
