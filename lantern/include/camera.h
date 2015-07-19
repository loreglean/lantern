#ifndef LANTERN_CAMERA_H
#define LANTERN_CAMERA_H

#include "vector3.h"

namespace lantern
{
	/** Class representing camera in 3D world */
	class camera final
	{
	public:
		/** Constructs camera with specified params
		* @param position Initial camera position
		* @param forward Initial camera direction, might be not normalized
		* @param fake_up Fake up vector which will be used to construct the real up vector from the forward vector, might be not normalized
		* @param horizontal_fov Camera's horizontal field of view in radians
		* @param aspect_ratio Viewport aspect ratio, used to calculate vertical field of view
		* @param near_plane_z Near plane z-coordinate
		* @param far_plane_z Far plane z-coordinate
		*/
		camera(
			vector3f const& position,
			vector3f const& forward,
			vector3f const& fake_up,
			float const horizontal_fov,
			float const aspect_ratio,
			float const near_plane_z,
			float const far_plane_z);

		/** Gets camera position
		* @returns Current position
		*/
		vector3f get_position() const;

		/** Sets camera position
		* @param position New camera position
		*/
		void set_position(vector3f const& position);

		/** Gets camera's z-axis coordinates
		* @returns Z-axis (forward vector)
		*/
		vector3f get_forward() const;

		/** Gets camera's x-axis coordinates
		* @returns X-axis (right vector)
		*/
		vector3f get_right() const;

		/** Gets camera's y-axis coordinates
		* @returns Y-axis (up vector)
		*/
		vector3f get_up() const;

		/** Gets horizontal field of view
		* @returns Horizontal field of view in radians
		*/
		float get_horizontal_fov() const;

		/** Gets vertical field of view
		* @returns Vertical field of view in radians
		*/
		float get_vertical_fov() const;

		/** Gets aspect ratio
		* @returns Aspect ratio (height / width)
		*/
		float get_aspect_ratio() const;

		/** Gets near plane z-coordinate
		* @returns Near plane z-coordinate
		*/
		float get_near_plane_z() const;

		/** Gets far plane z-coordinate
		* @returns Far plane z-coordinate
		*/
		float get_far_plane_z() const;

		/** Moves camera along its right vector
		* @param distance Distance to move
		*/
		void move_right(float const distance);

		/** Moves camera along its -right vector
		* @param distance Distance to move
		*/
		void move_left(float const distance);

		/** Moves camera along its up vector
		* @param distance Distance to move
		*/
		void move_up(float const distance);

		/** Moves camera along its -up vector
		* @param distance Distance to move
		*/
		void move_down(float const distance);

		/** Moves camera along its forward vector
		* @param distance Distance to move
		*/
		void move_forward(float const distance);

		/** Moves camera along its -forward vector
		* @param distance Distance to move
		*/
		void move_backward(float const distance);

		/** Rotates camera around y-axis
		* @param radians Radians to rotate for
		*/
		void yaw(float const radians);

		/** Rotates camera around x-axis
		* @param radians Radians to rotate for
		*/
		void pitch(float const radians);

	private:
		/** Current position */
		vector3f m_position;

		/** Current forward vector */
		vector3f m_forward;

		/** Current right vector */
		vector3f m_right;

		/** Current up vector */
		vector3f m_up;

		/** Horizontal field of view */
		float const m_horizontal_fov;

		/** Vertical field of view */
		float const m_vertical_fov;

		/** Aspect ratio */
		float const m_aspect_ratio;

		/** Near plane z-coordinate */
		float const m_near_plane_z;

		/** Far plane z-coordiante */
		float const m_far_plane_z;

		/** Creates coordinate system base on forward and fake up vector
		* @param fake_up Temp up vector
		*/
		void establish_coordinate_system(vector3f const& fake_up);
	};
}

#endif // LANTERN_CAMERA_H
