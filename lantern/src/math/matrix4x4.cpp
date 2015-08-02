#include <stddef.h>
#include "matrix4x4.h"

using namespace lantern;

matrix4x4f const matrix4x4f::IDENTITY = matrix4x4f{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f};

matrix4x4f::matrix4x4f()
{

}

matrix4x4f::matrix4x4f(float const m[4][4])
{
	for (int i{0}; i < 4; ++i)
	{
		for (int j{0}; j < 4; ++j)
		{
			this->values[i][j] = m[i][j];
		}
	}
}

matrix4x4f::matrix4x4f(
	float const m00, float const m01, float const m02, float const m03,
	float const m10, float const m11, float const m12, float const m13,
	float const m20, float const m21, float const m22, float const m23,
	float const m30, float const m31, float const m32, float const m33)
{
	this->values[0][0] = m00;
	this->values[0][1] = m01;
	this->values[0][2] = m02;
	this->values[0][3] = m03;

	this->values[1][0] = m10;
	this->values[1][1] = m11;
	this->values[1][2] = m12;
	this->values[1][3] = m13;

	this->values[2][0] = m20;
	this->values[2][1] = m21;
	this->values[2][2] = m22;
	this->values[2][3] = m23;

	this->values[3][0] = m30;
	this->values[3][1] = m31;
	this->values[3][2] = m32;
	this->values[3][3] = m33;
}

matrix4x4f matrix4x4f::operator*(matrix4x4f const& m) const
{
	matrix4x4f result;

	for (size_t i{0}; i < 4; ++i)
	{
		for (size_t j{0}; j < 4; ++j)
		{
			result.values[i][j] = 0.0f;
			for (size_t k{0}; k < 4; ++k)
			{
				result.values[i][j] += this->values[i][k] * m.values[k][j];
			}
		}
	}

	return result;
}

matrix4x4f matrix4x4f::translation(float const x, float const y, float const z)
{
	return matrix4x4f{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f};
}

matrix4x4f matrix4x4f::scale(float const x, float const y, float const z)
{
	return matrix4x4f{
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4f matrix4x4f::uniform_scale(float const s)
{
	return matrix4x4f{
		s, 0.0f, 0.0f, 0.0f,
		0.0f, s, 0.0f, 0.0f,
		0.0f, 0.0f, s, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4f matrix4x4f::rotation_around_x_axis(float const radians)
{
	return matrix4x4f{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(radians), std::sin(radians), 0.0f,
		0.0f, -std::sin(radians), std::cos(radians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4f matrix4x4f::rotation_around_y_axis(float const radians)
{
	return matrix4x4f{
		std::cos(radians), 0.0f, -std::sin(radians), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		std::sin(radians), 0.0f, std::cos(radians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4f matrix4x4f::rotation_around_z_axis(float const radians)
{
	return matrix4x4f{
		std::cos(radians), std::sin(radians), 0.0f, 0.0f,
		-std::sin(radians), std::cos(radians), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4f matrix4x4f::rotation_around_axis(vector3f const& axis, float const radians)
{
	vector3f axis_normalized{axis.normalized()};

	float const cos_value{std::cos(radians)};
	float const sin_value{std::sin(radians)};

	return matrix4x4f{
		axis_normalized.x * axis_normalized.x * (1.0f - cos_value) + cos_value,
		axis_normalized.x * axis_normalized.y * (1.0f - cos_value) + axis_normalized.z * sin_value,
		axis_normalized.x * axis_normalized.z * (1.0f - cos_value) - axis_normalized.y * sin_value,
		0.0f,

		axis_normalized.x * axis_normalized.y * (1.0f - cos_value) - axis_normalized.z * sin_value,
		axis_normalized.y * axis_normalized.y * (1.0f - cos_value) + cos_value,
		axis_normalized.y * axis_normalized.z * (1.0f - cos_value) + axis_normalized.x * sin_value,
		0.0f,

		axis_normalized.x * axis_normalized.z * (1.0f - cos_value) + axis_normalized.y * sin_value,
		axis_normalized.y * axis_normalized.z * (1.0f - cos_value) - axis_normalized.x * sin_value,
		axis_normalized.z * axis_normalized.z * (1.0f - cos_value) + cos_value,
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f};
}

matrix4x4f matrix4x4f::clip_space(float const hfov, float const vfov, float const near, float const far)
{
	float const projection_plane_z{1.0f};

	float const right{std::tan(hfov / 2.0f) * projection_plane_z};
	float const left{-right};
	float const top{std::tan(vfov / 2.0f) * projection_plane_z};
	float const bottom{-top};

	return matrix4x4f{
			2.0f * projection_plane_z / (right - left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f * projection_plane_z / (top - bottom), 0.0f, 0.0f,
			(left + right) / (left - right), (bottom + top) / (bottom - top), (far + near) / (far - near), 1.0f,
			0.0f, 0.0f, -2.0f * near * far / (far - near), 0.0f};
}
