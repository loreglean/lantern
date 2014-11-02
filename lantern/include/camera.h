#ifndef LANTERN_CAMERA_H
#define LANTERN_CAMERA_H

#include "vector3.h"

namespace lantern
{
	/** Class representing camera in 3D world.
	*/
	class camera final
	{
	public:
		camera(
			vector3 const& position,
			vector3 const& forward,
			vector3 const& fake_up,
			float const horizontal_fov,
			float const aspect_ratio,
			float const near_plane_z,
			float const far_plane_z);

		vector3 get_position() const;
		void set_position(vector3 const& position);

		vector3 get_forward() const;
		vector3 get_right() const;
		vector3 get_up() const;
		float get_horizontal_fov() const;
		float get_vertical_fov() const;
		float get_aspect_ratio() const;
		float get_near_plane_z() const;
		float get_far_plane_z() const;

		void move_right(float const distance);
		void move_left(float const distance);
		void move_up(float const distance);
		void move_down(float const distance);
		void move_forward(float const distance);
		void move_backward(float const distance);

		void yaw(float const radians);
		void pitch(float const radians);

	private:
		vector3 m_position;
		vector3 m_forward;
		vector3 m_right;
		vector3 m_up;
		float const m_horizontal_fov;
		float const m_vertical_fov;
		float const m_aspect_ratio;
		float const m_near_plane_z;
		float const m_far_plane_z;

		void establish_coordinate_system(vector3 const& fake_up);
	};
}

#endif // LANTERN_CAMERA_H
