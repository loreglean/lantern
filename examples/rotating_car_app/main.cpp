#include "app.h"
#include "camera.h"
#include "obj_import.h"
#include "pipeline.h"

using namespace lantern;

class rotating_car_app : public app
{
public:
	rotating_car_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float delta_since_last_frame) override;
	void on_key_down(SDL_Keysym key) override;

private:
	void update_shader_mvp();

	vector3 m_car_position;
	vector3 m_car_rotation;
	mesh m_car_mesh;

	camera m_camera;
	color_shader m_shader;
	pipeline m_pipeline;
};

rotating_car_app::rotating_car_app(unsigned int const width, unsigned int const height)
	: app(width, height),
	  m_car_position{0.0f, 0.0f, 5.0f},
	  m_car_rotation{static_cast<float>(M_PI) / 8.0f, static_cast<float>(M_PI), 0.0f},
	  m_car_mesh{load_mesh_from_obj("resources/car.obj", false, false)},
	  m_camera{
		  vector3::ZERO,
		  vector3::Z_UNIT,
		  vector3::Y_UNIT,
		  static_cast<float>(M_PI) / 2.0f,
		  static_cast<float>(height) / static_cast<float>(width),
		  0.01f,
		  20.0f}
{
	// Update model-view-projection matrix for the first time
	update_shader_mvp();

	// Create colors attribute
	//
	std::vector<color> colors{ color{255, 255, 255} };
	std::vector<unsigned int> colors_indices;
	for (size_t i = 0; i < m_car_mesh.get_vertices().size(); ++i)
	{
		// All vertices use the same color
		colors_indices.push_back(0);
	}
	mesh_attribute_info<color> color{"colors", colors, colors_indices};
	m_car_mesh.get_color_attributes_storage().push_back(color);

	// Bind data to pipeline
	//
	m_pipeline.bind_vertices(&m_car_mesh.get_vertices());
	m_pipeline.bind_indices(&m_car_mesh.get_indices());
	m_pipeline.bind_shader(&m_shader);
	m_pipeline.bind_color_attribute(&m_car_mesh.get_color_attributes_storage()[0], m_shader.get_color_attr_bind_point());
}

void rotating_car_app::frame(float delta_since_last_frame)
{
	// Draw
	m_pipeline.draw(get_painter());
}

void rotating_car_app::on_key_down(SDL_Keysym key)
{
	app::on_key_down(key);

	float const moving_speed{0.1f};
	float const rotation_speed{0.05f};

	if (key.sym == SDLK_a)
	{
		m_camera.move_left(moving_speed);
	}
	else if (key.sym == SDLK_d)
	{
		m_camera.move_right(moving_speed);
	}
	else if (key.sym == SDLK_w)
	{
		m_camera.move_forward(moving_speed);
	}
	else if (key.sym == SDLK_s)
	{
		m_camera.move_backward(moving_speed);
	}
	else if (key.sym == SDLK_r)
	{
		m_camera.move_up(moving_speed);
	}
	else if (key.sym == SDLK_f)
	{
		m_camera.move_down(moving_speed);
	}
	else if (key.sym == SDLK_q)
	{
		m_camera.yaw(-rotation_speed);
	}
	else if (key.sym == SDLK_e)
	{
		m_camera.yaw(rotation_speed);
	}

	// Update model-view-projection according to camera changes
	update_shader_mvp();
}

void rotating_car_app::update_shader_mvp()
{
	matrix4x4 const local_to_world_transform{
		matrix4x4::rotation_around_x_axis(m_car_rotation.x) *
		matrix4x4::rotation_around_y_axis(m_car_rotation.y) *
		matrix4x4::rotation_around_z_axis(m_car_rotation.z) *
		matrix4x4::translation(m_car_position.x, m_car_position.y, m_car_position.z)};

	matrix4x4 const camera_rotation{
		m_camera.get_right().x, m_camera.get_up().x, m_camera.get_forward().x, 0.0f,
		m_camera.get_right().y, m_camera.get_up().y, m_camera.get_forward().y, 0.0f,
		m_camera.get_right().z, m_camera.get_up().z, m_camera.get_forward().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};

	matrix4x4 const camera_translation{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-m_camera.get_position().x, -m_camera.get_position().y, -m_camera.get_position().z, 1.0f};

	matrix4x4 const world_to_camera_transform{camera_translation * camera_rotation};

	matrix4x4 const camera_to_clip_transform{
		matrix4x4::clip_space(
			m_camera.get_horizontal_fov(),
			m_camera.get_vertical_fov(),
			m_camera.get_near_plane_z(),
			m_camera.get_far_plane_z())};

	matrix4x4 const local_to_clip_transform{
		local_to_world_transform * world_to_camera_transform * camera_to_clip_transform};

	m_shader.set_mvp_matrix(local_to_clip_transform);
}

int main(int argc, char* argv[])
{
	return rotating_car_app{640, 480}.start();
}

