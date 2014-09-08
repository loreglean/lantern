#include <gtest/gtest.h>

#include "vector4.h"

using namespace lantern;

#define ASSERT_FLOATS_NEAR(f1, f2) ASSERT_NEAR(f1, f2, 0.0001f)

TEST(vector4, constructor)
{
    vector4 v(2.0f, -4.0f, 15.0f, -0.5f);

    ASSERT_FLOATS_NEAR(v.x, 2.0f);
    ASSERT_FLOATS_NEAR(v.y, -4.0f);
    ASSERT_FLOATS_NEAR(v.z, 15.0f);
    ASSERT_FLOATS_NEAR(v.w, -0.5f);
}
