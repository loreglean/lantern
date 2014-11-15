#include "common.h"
#include "vector3.h"

using namespace lantern;

TEST(vector3, constructors)
{
	vector3 const v{2.0f, -4.0f, 15.0f};
	ASSERT_FLOATS_NEAR(v.x, 2.0f);
	ASSERT_FLOATS_NEAR(v.y, -4.0f);
	ASSERT_FLOATS_NEAR(v.z, 15.0f);

	vector3 const v_default{};
	ASSERT_FLOATS_NEAR(v_default.x, 0.0f);
	ASSERT_FLOATS_NEAR(v_default.y, 0.0f);
	ASSERT_FLOATS_NEAR(v_default.z, 0.0f);
}

TEST(vector3, negation)
{
	vector3 const v{13.0f, -99.0f, 0.0f};
	vector3 const v_neg{-v};
	ASSERT_FLOATS_NEAR(v_neg.x, -13.0f);
	ASSERT_FLOATS_NEAR(v_neg.y, 99.0f);
	ASSERT_FLOATS_NEAR(v_neg.z, 0.0f);
}

TEST(vector3, addition)
{
	vector3 const v{-15.0f, 0.0f, 7.0f};
	vector3 const w{15.0f, -5.0f, 13.0f};

	vector3 v_w_added{v + w};
	ASSERT_FLOATS_NEAR(v_w_added.x, 0.0f);
	ASSERT_FLOATS_NEAR(v_w_added.y, -5.0f);
	ASSERT_FLOATS_NEAR(v_w_added.z, 20.0f);

	v_w_added = v;
	v_w_added += w;
	ASSERT_FLOATS_NEAR(v_w_added.x, 0.0f);
	ASSERT_FLOATS_NEAR(v_w_added.y, -5.0f);
	ASSERT_FLOATS_NEAR(v_w_added.z, 20.0f);
}

TEST(vector3, subtraction)
{
	vector3 const v{-15.0f, 0.0f, 7.0f};
	vector3 const w{15.0f, -5.0f, 13.0f};

	vector3 v_w_subtracted{v - w};
	ASSERT_FLOATS_NEAR(v_w_subtracted.x, -30.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.y, 5.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.z, -6.0f);

	v_w_subtracted = v;
	v_w_subtracted -= w;
	ASSERT_FLOATS_NEAR(v_w_subtracted.x, -30.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.y, 5.0f);
	ASSERT_FLOATS_NEAR(v_w_subtracted.z, -6.0f);
}

TEST(vector3, multiplication_by_scalar)
{
	vector3 const v{1.0f, -2.0f, 3.0f};
	
	vector3 v_muled{v * 6.0f};
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

TEST(vector3, division_by_scalar)
{
	vector3 const v{1.0f, -2.0f, 3.0f};

	vector3 v_divided{v / 5.0f};
	ASSERT_FLOATS_NEAR(v_divided.x, 0.2f);
	ASSERT_FLOATS_NEAR(v_divided.y, -0.4f);
	ASSERT_FLOATS_NEAR(v_divided.z, 0.6f);

	v_divided = v;
	v_divided /= 5.0f;
	ASSERT_FLOATS_NEAR(v_divided.x, 0.2f);
	ASSERT_FLOATS_NEAR(v_divided.y, -0.4f);
	ASSERT_FLOATS_NEAR(v_divided.z, 0.6f);
}

TEST(vector3, length)
{
	vector3 const v{5.0f, -4.0f, 7.0f};
	ASSERT_FLOATS_NEAR(v.length(), 9.4868f);
    ASSERT_FLOATS_NEAR(v.length_sqr(), 90.0f);
}

TEST(vector3, normalization)
{
	vector3 v(13.0f, -5.0f, 0.55f);

	v.normalize();
	ASSERT_FLOATS_NEAR(v.x, 0.9326f);
	ASSERT_FLOATS_NEAR(v.y, -0.3587f);
	ASSERT_FLOATS_NEAR(v.z, 0.0394f);
	ASSERT_FLOATS_NEAR(v.length(), 1.0f);

	vector3 const v_normalized{v.normalized()};
	ASSERT_FLOATS_NEAR(v_normalized.x, 0.9326f);
	ASSERT_FLOATS_NEAR(v_normalized.y, -0.3587f);
	ASSERT_FLOATS_NEAR(v_normalized.z, 0.0394f);
	ASSERT_FLOATS_NEAR(v_normalized.length(), 1.0f);
}

TEST(vector3, dot_product)
{
	vector3 const v{12.0f, -3.0f, 5.0f};
	vector3 const w{-5.0f, -3.0f, 0.5f};
	ASSERT_FLOATS_NEAR(v.dot(w), -48.5f);
}

TEST(vector3, cross_product)
{
	vector3 const v{-5.0f, 17.0f, 3.0f};
	vector3 const w{3.0f, 12.0f, 0.0f};
	vector3 cross{v.cross(w)};
	ASSERT_FLOATS_NEAR(cross.x, -36.0f);
	ASSERT_FLOATS_NEAR(cross.y, 9.0f);
	ASSERT_FLOATS_NEAR(cross.z, -111.0f);
}

TEST(vector3, distance_between_points)
{
	vector3 const p1{5.0f, 5.0f, 5.0f};
	vector3 const p2{0.0f, 2.0f, 3.0f};
	ASSERT_FLOATS_NEAR(p1.distance_to(p2), 6.1644f);
}

TEST(vector3, angle_between_vectors)
{
	vector3 const v1{1.0f, 2.0f, 5.0f};
	vector3 const v2{3.0f, 0.0f, 2.0f};
	ASSERT_FLOATS_NEAR(v1.angle_with(v2), 0.8522f);
}

TEST(vector3, projection_on_vector)
{
	vector3 const v1{0.3f, 1.0f, 2.0f};
	vector3 const v2{-1.0f, -0.3f, 5.0f};
	vector3 const projection{v1.projection_on(v2)};
	ASSERT_FLOATS_NEAR(projection.x, -0.3602f);
	ASSERT_FLOATS_NEAR(projection.y, -0.1080f);
	ASSERT_FLOATS_NEAR(projection.z, 1.8014f);
}

TEST(vector3, perpendicular_on_vector)
{
	vector3 const v1{1.0f, 2.0f, 1.0f};
	vector3 const v2{-1.0f, 3.0f, 2.0f};
	vector3 const perpendicular{v1.perpendicular_on(v2)};
	ASSERT_FLOATS_NEAR(perpendicular.x, 1.5f);
	ASSERT_FLOATS_NEAR(perpendicular.y, 0.5f);
	ASSERT_FLOATS_NEAR(perpendicular.z, 0.0f);
}
