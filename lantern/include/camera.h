#ifndef LANTERN_CAMERA_H
#define LANTERN_CAMERA_H

#include "vector3.h"

namespace lantern
{
	/** Class representing camera in 3D world.
	*/
	class camera
	{
	public:
		vector3 position;

		camera(vector3 position, vector3 forward);

		vector3 get_forward() const;
		vector3 get_right() const;
		vector3 get_up() const;

		void move_right(float const distance);
		void move_left(float const distance);
		void move_up(float const distance);
		void move_down(float const distance);
		void move_forward(float const distance);
		void move_backward(float const distance);

		void yaw(float const radians);
		void pitch(float const radians);

	private:
		vector3 m_forward;
		vector3 m_right;
		vector3 m_up;

		void establish_coordinate_system();
	};
}

#endif // LANTERN_CAMERA_H
