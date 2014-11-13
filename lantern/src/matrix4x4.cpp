#include "matrix4x4.h"

using namespace lantern;

matrix4x4 const matrix4x4::IDENTITY = matrix4x4{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f};

matrix4x4::matrix4x4()
	: values{{0.0f}}
{
}

matrix4x4::matrix4x4(float m[4][4])
{
	// values = m;
	for (int i=0; i<4; ++i) {
		for (int j=0; j<4; ++j) {
			values[i][j] = m[i][j];
		}
	}
}

matrix4x4::matrix4x4(
	float const m00, float const m01, float const m02, float const m03,
	float const m10, float const m11, float const m12, float const m13,
	float const m20, float const m21, float const m22, float const m23,
	float const m30, float const m31, float const m32, float const m33)
{
	values[0][0] = m00;
	values[0][1] = m01;
	values[0][2] = m02;
	values[0][3] = m03;

	values[1][0] = m10;
	values[1][1] = m11;
	values[1][2] = m12;
	values[1][3] = m13;

	values[2][0] = m20;
	values[2][1] = m21;
	values[2][2] = m22;
	values[2][3] = m23;

	values[3][0] = m30;
	values[3][1] = m31;
	values[3][2] = m32;
	values[3][3] = m33;
}

matrix4x4 matrix4x4::operator*(matrix4x4 const &m) const
{
	float temp[4][4];
	for(int i=0; i<4; i++) {
		for(int l=0; l<4; l++) {
			float s = 0;
			for(int j=0; j<4; j++) {
				s += values[i][j]*m.values[j][l];
			}
			temp[i][l] = s;
		}
	}
	return matrix4x4(temp);
}

matrix4x4 matrix4x4::translation(float const x, float const y, float const z)
{
	return matrix4x4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f};
}

matrix4x4 matrix4x4::scale(float const x, float const y, float const z)
{
	return matrix4x4{
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4 matrix4x4::uniform_scale(float const s)
{
	return matrix4x4{
		s, 0.0f, 0.0f, 0.0f,
		0.0f, s, 0.0f, 0.0f,
		0.0f, 0.0f, s, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4 matrix4x4::rotation_around_x_axis(float const radians)
{
	return matrix4x4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(radians), std::sin(radians), 0.0f,
		0.0f, -std::sin(radians), std::cos(radians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4 matrix4x4::rotation_around_y_axis(float const radians)
{
	return matrix4x4{
		std::cos(radians), 0.0f, -std::sin(radians), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		std::sin(radians), 0.0f, std::cos(radians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4 matrix4x4::rotation_around_z_axis(float const radians)
{
	return matrix4x4{
		std::cos(radians), std::sin(radians), 0.0f, 0.0f,
		-std::sin(radians), std::cos(radians), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
}

matrix4x4 matrix4x4::rotation_around_axis(vector3 const& axis, float const radians)
{
	vector3 axis_normalized{axis.normalized()};

	float const cos_value{std::cos(radians)};
	float const sin_value{std::sin(radians)};

	return matrix4x4{
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

vector4 lantern::operator*(vector4 const& v, matrix4x4 const& m)
{
	return vector4{
		v.x * m.values[0][0] + v.y * m.values[1][0] + v.z * m.values[2][0] + v.w * m.values[3][0],
		v.x * m.values[0][1] + v.y * m.values[1][1] + v.z * m.values[2][1] + v.w * m.values[3][1],
		v.x * m.values[0][2] + v.y * m.values[1][2] + v.z * m.values[2][2] + v.w * m.values[3][2],
		v.x * m.values[0][3] + v.y * m.values[1][3] + v.z * m.values[2][3] + v.w * m.values[3][3]};
}
