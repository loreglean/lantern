#include "camera.h"
#include "matrix3x3.h"

using namespace lantern;

camera::camera(
	vector3 const& position,
	vector3 const& forward,
	vector3 const& fake_up,
	float const horizontal_fov,
	float const aspect_ratio,
	float const near_plane_z,
	float const far_plane_z)
	: m_position{position},
	  m_forward{forward.normalized()},
	  m_horizontal_fov{horizontal_fov},
	  m_vertical_fov{horizontal_fov * aspect_ratio},
	  m_aspect_ratio{aspect_ratio},
	  m_near_plane_z{near_plane_z},
	  m_far_plane_z{far_plane_z}
{
	establish_coordinate_system(fake_up.normalized());
}

vector3 camera::get_position() const
{
	return m_position;
}

void camera::set_position(vector3 const& position)
{
	m_position = position;
}

vector3 camera::get_forward() const
{
	return m_forward;
}

vector3 camera::get_right() const
{
	return m_right;
}

vector3 camera::get_up() const
{
	return m_up;
}

float camera::get_horizontal_fov() const
{
	return m_horizontal_fov;
}

float camera::get_vertical_fov() const
{
	return m_vertical_fov;
}

float camera::get_aspect_ratio() const
{
	return m_aspect_ratio;
}

float camera::get_near_plane_z() const
{
	return m_near_plane_z;
}

float camera::get_far_plane_z() const
{
	return m_far_plane_z;
}

void camera::establish_coordinate_system(vector3 const& fake_up)
{
	// fake_up vector does not represent the up vector itself
	// It might be different - together with m_forward it represents the plane where the correct up vector should be
	// So we use it to calculate right vector and then fix it
	//

	m_right = fake_up.cross(m_forward).normalized();
	m_up = m_forward.cross(m_right).normalized();
}

void camera::move_right(float distance)
{
	m_position += m_right * distance;
}

void camera::move_left(float const distance)
{
	move_right(-distance);
}

void camera::move_up(float const distance)
{
	m_position += m_up * distance;
}

void camera::move_down(float const distance)
{
	move_up(-distance);
}

void camera::move_forward(float const distance)
{
	m_position += m_forward * distance;
}

void camera::move_backward(float const distance)
{
	move_forward(-distance);
}

void camera::yaw(float const radians)
{
	matrix3x3 const rotation{matrix3x3::rotation_around_y_axis(radians)};
	m_forward = m_forward * rotation;
	m_right = m_right * rotation;
	m_up = m_up * rotation;
}

void camera::pitch(float const radians)
{
	matrix3x3 const rotation{matrix3x3::rotation_around_x_axis(radians)};
	m_forward = m_forward * rotation;
	m_right = m_right * rotation;
	m_up = m_up * rotation;
}
