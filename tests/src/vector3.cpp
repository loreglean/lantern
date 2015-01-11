#include "assert_utils.h"

using namespace lantern;

TEST(vector3, constructors)
{
	vector3 const v{2.0f, -4.0f, 15.0f};
	assert_floats_near(v.x, 2.0f);
	assert_floats_near(v.y, -4.0f);
	assert_floats_near(v.z, 15.0f);

	vector3 const v_default{};
	assert_floats_near(v_default.x, 0.0f);
	assert_floats_near(v_default.y, 0.0f);
	assert_floats_near(v_default.z, 0.0f);
}

TEST(vector3, negation)
{
	vector3 const v{13.0f, -99.0f, 0.0f};
	vector3 const v_neg{-v};
	assert_vectors3_near(v_neg, vector3{-13.0f, 99.0f, 0.0f});
}

TEST(vector3, addition)
{
	vector3 const v{-15.0f, 0.0f, 7.0f};
	vector3 const w{15.0f, -5.0f, 13.0f};

	vector3 v_w_added{v + w};
	assert_vectors3_near(v_w_added, vector3{0.0f, -5.0f, 20.0f});

	v_w_added = v;
	v_w_added += w;
	assert_vectors3_near(v_w_added, vector3{0.0f, -5.0f, 20.0f});
}

TEST(vector3, subtraction)
{
	vector3 const v{-15.0f, 0.0f, 7.0f};
	vector3 const w{15.0f, -5.0f, 13.0f};

	vector3 v_w_subtracted{v - w};
	assert_vectors3_near(v_w_subtracted, vector3{-30.0f, 5.0f, -6.0f});

	v_w_subtracted = v;
	v_w_subtracted -= w;
	assert_vectors3_near(v_w_subtracted, vector3{-30.0f, 5.0f, -6.0f});
}

TEST(vector3, multiplication_by_scalar)
{
	vector3 const v{1.0f, -2.0f, 3.0f};
	
	vector3 v_muled{v * 6.0f};
	assert_vectors3_near(v_muled, vector3{6.0f, -12.0f, 18.0f});

	v_muled = 6.0f * v;
	assert_vectors3_near(v_muled, vector3{6.0f, -12.0f, 18.0f});

	v_muled = v;
	v_muled *= 6.0f;
	assert_vectors3_near(v_muled, vector3{6.0f, -12.0f, 18.0f});
}

TEST(vector3, division_by_scalar)
{
	vector3 const v{1.0f, -2.0f, 3.0f};

	vector3 v_divided{v / 5.0f};
	assert_vectors3_near(v_divided, vector3{0.2f, -0.4f, 0.6f});

	v_divided = v;
	v_divided /= 5.0f;
	assert_vectors3_near(v_divided, vector3{0.2f, -0.4f, 0.6f});
}

TEST(vector3, length)
{
	vector3 const v{5.0f, -4.0f, 7.0f};
	assert_floats_near(v.length(), 9.4868f);
	assert_floats_near(v.length_sqr(), 90.0f);
}

TEST(vector3, normalization)
{
	vector3 v(13.0f, -5.0f, 0.55f);

	v.normalize();
	assert_vectors3_near(v, vector3{0.9326f, -0.3587f, 0.0394f});
	assert_floats_near(v.length(), 1.0f);

	vector3 const v_normalized{v.normalized()};
	assert_vectors3_near(v_normalized, vector3{0.9326f, -0.3587f, 0.0394f});
	assert_floats_near(v_normalized.length(), 1.0f);
}

TEST(vector3, dot_product)
{
	vector3 const v{12.0f, -3.0f, 5.0f};
	vector3 const w{-5.0f, -3.0f, 0.5f};
	assert_floats_near(v.dot(w), -48.5f);
}

TEST(vector3, cross_product)
{
	vector3 const v{-5.0f, 17.0f, 3.0f};
	vector3 const w{3.0f, 12.0f, 0.0f};
	vector3 cross{v.cross(w)};
	assert_vectors3_near(cross, vector3{-36.0f, 9.0f, -111.0f});
}

TEST(vector3, distance_between_points)
{
	vector3 const p1{5.0f, 5.0f, 5.0f};
	vector3 const p2{0.0f, 2.0f, 3.0f};
	assert_floats_near(p1.distance_to(p2), 6.1644f);
}

TEST(vector3, angle_between_vectors)
{
	vector3 const v1{1.0f, 2.0f, 5.0f};
	vector3 const v2{3.0f, 0.0f, 2.0f};
	assert_floats_near(v1.angle_with(v2), 0.8522f);
}

TEST(vector3, projection_on_vector)
{
	vector3 const v1{0.3f, 1.0f, 2.0f};
	vector3 const v2{-1.0f, -0.3f, 5.0f};
	vector3 const projection{v1.projection_on(v2)};
	assert_vectors3_near(projection, vector3{-0.3602f, -0.1080f, 1.8014f});
}

TEST(vector3, perpendicular_on_vector)
{
	vector3 const v1{1.0f, 2.0f, 1.0f};
	vector3 const v2{-1.0f, 3.0f, 2.0f};
	vector3 const perpendicular{v1.perpendicular_on(v2)};
	assert_vectors3_near(perpendicular, vector3{1.5f, 0.5f, 0.0f});
}
