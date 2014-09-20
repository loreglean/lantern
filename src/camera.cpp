#include "camera.h"
#include "matrix3x3.h"

using namespace lantern;

camera::camera(vector3 position, vector3 forward)
	: m_position{position}, m_forward{forward.normalized()}
{
	establish_coordinate_system();
}

vector3 camera::get_position() const
{
	return m_position;
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

void camera::set_position(vector3 const &position)
{
	m_position = position;
}

void camera::establish_coordinate_system()
{
	// We need to create coordinate system based on forward vector we got:
	// forward is perpendicular to right, forward is perpendicular to up, and right is perpendicular to up.
	//

	// But we have only forward now, so we will do the trick.
	// First, we will compute up vector.
	// To do that we will assume for now that right vector is (1, 0, 0) (call it right') and will take their cross product.
	// Now we have: forward is perpendicular to up, right' is perpendicular to up.
	//

	vector3 fake_right{1.0f, 0.0f, 0.0f};
	m_up = m_forward.cross(fake_right).normalized();

	// Although we now have both forward and up, we have to compute correct right vector.
	// For now up is perpendicular to right', but forward isn't.
	// To get vector that is perpendicular to both of them we take their cross product.
	// Thus getting: forward is perpendicular to up, forward is perpendicular to right, up is perpendicular to right.
	m_right = m_up.cross(m_forward).normalized();
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
