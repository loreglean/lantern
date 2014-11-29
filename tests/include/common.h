#ifndef LANTERN_TESTS_COMMON_H
#define LANTERN_TESTS_COMMON_H

#include "gtest/gtest.h"
#include "vector2.h"
#include "vector3.h"

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

#endif // LANTERN_TESTS_COMMON_H
