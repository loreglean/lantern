#include <gtest/gtest.h>

#include "vector3.h"

using namespace lantern;

#define ASSERT_FLOATS_NEAR(f1, f2) ASSERT_NEAR(f1, f2, 0.0001f)

TEST(vector3, constructor)
{
	vector3 v(2.0f, -4.0f, 15.0f);
	
	ASSERT_FLOATS_NEAR(v.x, 2.0f);
	ASSERT_FLOATS_NEAR(v.y, -4.0f);
	ASSERT_FLOATS_NEAR(v.z, 15.0f);
}

TEST(vector3, negation)
{
	vector3 v(13.0f, -99.0f, 0.0f);
	
	vector3 v_neg = -v;
	ASSERT_FLOATS_NEAR(v_neg.x, -13.0f);
	ASSERT_FLOATS_NEAR(v_neg.y, 99.0f);
	ASSERT_FLOATS_NEAR(v_neg.z, 0.0f);
}

TEST(vector3, multiplication_by_scalar)
{
	vector3 v(1.0f, -2.0f, 3.0f);
	
	vector3 v_muled = v * 6.0f;
	ASSERT_FLOATS_NEAR(v_muled.x, 6.0f);
	ASSERT_FLOATS_NEAR(v_muled.y, -12.0f);
	ASSERT_FLOATS_NEAR(v_muled.z, 18.0f);

	v_muled = 6.0f * v;
	ASSERT_FLOATS_NEAR(v_muled.x, 6.0f);
	ASSERT_FLOATS_NEAR(v_muled.y, -12.0f);
	ASSERT_FLOATS_NEAR(v_muled.z, 18.0f);

	v_muled = v;
	v_muled *= 6.0f;
	ASSERT_FLOATS_NEAR(v_muled.x, 6.0f);
	ASSERT_FLOATS_NEAR(v_muled.y, -12.0f);
	ASSERT_FLOATS_NEAR(v_muled.z, 18.0f);
}

TEST(vector3, addition_subtracton)
{
	vector3 v(-15.0f, 0.0f, 7.0f);
	vector3 w(15.0f, -5.0f, 13.0f);

	vector3 v_w_added = v + w;
	ASSERT_FLOATS_NEAR(v_w_added.x, 0.0f);
	ASSERT_FLOATS_NEAR(v_w_added.y, -5.0f);
	ASSERT_FLOATS_NEAR(v_w_added.z, 20.0f);

	v_w_added = v;
	v_w_added += w;
	ASSERT_FLOATS_NEAR(v_w_added.x, 0.0f);
	ASSERT_FLOATS_NEAR(v_w_added.y, -5.0f);
	ASSERT_FLOATS_NEAR(v_w_added.z, 20.0f);

	vector3 v_w_subtracted = v - w;
	ASSERT_FLOATS_NEAR(v_w_subtracted.x, -30.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.y, 5.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.z, -6.0f);

	v_w_subtracted = v;
	v_w_subtracted -= w;
	ASSERT_FLOATS_NEAR(v_w_subtracted.x, -30.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.y, 5.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.z, -6.0f);
}

TEST(vector3, length)
{
	vector3 v(5.0f, -4.0f, 7.0f);

	ASSERT_FLOATS_NEAR(v.length(), 9.4868f);
}

TEST(vector3, normalization)
{
	vector3 v(13.0f, -5.0f, 0.55f);

	v.normalize();
	ASSERT_FLOATS_NEAR(v.x, 0.9326f);
	ASSERT_FLOATS_NEAR(v.y, -0.3587f);
	ASSERT_FLOATS_NEAR(v.z, 0.0394f);
	ASSERT_FLOATS_NEAR(v.length(), 1.0f);
}

TEST(vector3, dot_product)
{
	vector3 v(12.0f, -3.0f, 5.0f);
	vector3 w(-5.0f, -3.0f, 0.5f);

	ASSERT_FLOATS_NEAR(v.dot(w), -48.5f);
}

TEST(vector3, cross_product)
{
	vector3 v(-5.0f, 17.0f, 3.0f);
	vector3 w(3.0f, 12.0f, 0.0f);

	vector3 cross = v.cross(w);
	ASSERT_FLOATS_NEAR(cross.x, -36.0f);
	ASSERT_FLOATS_NEAR(cross.y, 9.0f);
	ASSERT_FLOATS_NEAR(cross.z, -111.0f);
}

TEST(vector3, distance_between_points)
{
	vector3 p1(5.0f, 5.0f, 5.0f);
	vector3 p2(0.0f, 2.0f, 3.0f);

	ASSERT_FLOATS_NEAR(p1.distance_to(p2), 6.1644f);
}
