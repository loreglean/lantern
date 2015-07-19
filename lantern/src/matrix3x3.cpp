#include <stddef.h>
#include "matrix3x3.h"

using namespace lantern;

matrix3x3 const matrix3x3::IDENTITY = matrix3x3{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f};

matrix3x3::matrix3x3()
{

}

matrix3x3::matrix3x3(float const array[3][3])
{
	for (size_t i{0}; i < 3; ++i)
	{
		for (size_t j{0}; j < 3; ++j)
		{
			values[i][j] = array[i][j];
		}
	}
}

matrix3x3::matrix3x3(
	float const m00, float const m01, float const m02,
	float const m10, float const m11, float const m12,
	float const m20, float const m21, float const m22)
{
	values[0][0] = m00;
	values[0][1] = m01;
	values[0][2] = m02;

	values[1][0] = m10;
	values[1][1] = m11;
	values[1][2] = m12;

	values[2][0] = m20;
	values[2][1] = m21;
	values[2][2] = m22;
}

matrix3x3 matrix3x3::operator*(matrix3x3 const& m) const
{
	matrix3x3 result;

	for (size_t i{0}; i < 3; ++i)
	{
		for (size_t j{0}; j < 3; ++j)
		{
			result.values[i][j] = 0.0f;
			for (size_t k{0}; k < 3; ++k)
			{
				result.values[i][j] += values[i][k] * m.values[k][j];
			}
		}
	}

	return result;
}

float matrix3x3::det() const
{
	return
		values[0][0] * (values[1][1] * values[2][2] - values[1][2] * values[2][1]) -
		values[0][1] * (values[1][0] * values[2][2] - values[1][2] * values[2][0]) +
		values[0][2] * (values[1][0] * values[2][1] - values[1][1] * values[2][0]);
}

matrix3x3 matrix3x3::inversed() const
{
	return inversed_precalc_det(det());
}

matrix3x3 matrix3x3::inversed_precalc_det(float const det) const
{
	matrix3x3 result;

	result.values[0][0] = values[1][1] * values[2][2] - values[1][2] * values[2][1];
	result.values[1][0] = values[1][2] * values[2][0] - values[1][0] * values[2][2];
	result.values[2][0] = values[1][0] * values[2][1] - values[1][1] * values[2][0];

	result.values[0][1] = values[0][2] * values[2][1] - values[0][1] * values[2][2];
	result.values[1][1] = values[0][0] * values[2][2] - values[0][2] * values[2][0];
	result.values[2][1] = values[0][1] * values[2][0] - values[0][0] * values[2][1];

	result.values[0][2] = values[0][1] * values[1][2] - values[0][2] * values[1][1];
	result.values[1][2] = values[0][2] * values[1][0] - values[0][0] * values[1][2];
	result.values[2][2] = values[0][0] * values[1][1] - values[0][1] * values[1][0];

	float const det_ivnersed = 1.0f / det;
	for (size_t j{0}; j < 3; ++j)
	{
		for (size_t i{0}; i < 3; ++i)
		{
			result.values[i][j] *= det_ivnersed;
		}
	}

	return result;
}

matrix3x3 matrix3x3::scale(float const x, float const y, float const z)
{
	return matrix3x3{
		x, 0.0f, 0.0f,
		0.0f, y, 0.0f,
		0.0f, 0.0f, z};
}

matrix3x3 matrix3x3::uniform_scale(float const s)
{
	return matrix3x3{
		s, 0.0f, 0.0f,
		0.0f, s, 0.0f,
		0.0f, 0.0f, s};
}

matrix3x3 matrix3x3::rotation_around_x_axis(float const radians)
{
	return matrix3x3{
		1.0f, 0.0f, 0.0f,
		0.0f, std::cos(radians), std::sin(radians),
		0.0f, -std::sin(radians), std::cos(radians)};
}

matrix3x3 matrix3x3::rotation_around_y_axis(float const radians)
{
	return matrix3x3{
		std::cos(radians), 0.0f, -std::sin(radians),
		0.0f, 1.0f, 0.0f,
		std::sin(radians), 0.0f, std::cos(radians)};
}

matrix3x3 matrix3x3::rotation_around_z_axis(float const radians)
{
	return matrix3x3{
		std::cos(radians), std::sin(radians), 0.0f,
		-std::sin(radians), std::cos(radians), 0.0f,
		0.0f, 0.0f, 1.0f};
}

matrix3x3 matrix3x3::rotation_around_axis(vector3f const& axis, float const radians)
{
	vector3f axis_normalized{axis.normalized()};

	float const cos_value{std::cos(radians)};
	float const sin_value{std::sin(radians)};

	return matrix3x3{
		axis_normalized.x * axis_normalized.x * (1.0f - cos_value) + cos_value,
		axis_normalized.x * axis_normalized.y * (1.0f - cos_value) + axis_normalized.z * sin_value,
		axis_normalized.x * axis_normalized.z * (1.0f - cos_value) - axis_normalized.y * sin_value,

		axis_normalized.x * axis_normalized.y * (1.0f - cos_value) - axis_normalized.z * sin_value,
		axis_normalized.y * axis_normalized.y * (1.0f - cos_value) + cos_value,
		axis_normalized.y * axis_normalized.z * (1.0f - cos_value) + axis_normalized.x * sin_value,

		axis_normalized.x * axis_normalized.z * (1.0f - cos_value) + axis_normalized.y * sin_value,
		axis_normalized.y * axis_normalized.z * (1.0f - cos_value) - axis_normalized.x * sin_value,
		axis_normalized.z * axis_normalized.z * (1.0f - cos_value) + cos_value};
}
