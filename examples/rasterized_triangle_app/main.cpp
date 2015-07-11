#include "app.h"
#include "obj_import.h"
#include "camera.h"
#include "color_shader.h"
#include "texture_shader.h"

using namespace lantern;

enum class shader_option
{
	color,
	texture
};

/** Draws simple triangle with interpolated color attribute */
class rasterized_color_triangle_app : public app
{
public:
	rasterized_color_triangle_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float const delta_since_last_frame) override;
	void on_key_down(SDL_Keysym const key) override;

private:
	/** Updates view-model-projection matrix and gives it to the shader */
	void update_shader_mvp();

	vector3 const m_triangle_position;
	vector3 const m_triangle_rotation;
	mesh m_triangle_mesh;

	camera m_camera;
	
	color_shader m_color_shader;
	texture_shader m_texture_shader;
	shader_option m_shader_option;

	texture m_texture;
};

rasterized_color_triangle_app::rasterized_color_triangle_app(unsigned int const width, unsigned int const height)
	: app(width, height),
	  m_triangle_position{0.0f, 0.0f, 1.5f},
	  m_triangle_rotation{vector3::ZERO},
	  m_triangle_mesh{load_mesh_from_obj("resources/triangle.obj", false, false)},
	  m_camera{
		  vector3::ZERO,
		  vector3::Z_UNIT,
		  vector3::Y_UNIT,
		  static_cast<float>(M_PI) / 2.0f,
		  static_cast<float>(height) / static_cast<float>(width),
		  0.01f,
		  20.0f},
	  m_shader_option{shader_option::color},
	  m_texture{texture::load_from_file("resources/chess.png")}
{
	// Update model-view-projection matrix for the first time
	update_shader_mvp();

	std::vector<unsigned int> const indices{0, 1, 2};

	// Add color attribute to triangle mesh
	//
	std::vector<color> const colors{color::GREEN, color::RED, color::BLUE};
	mesh_attribute_info<color> const color_info{VERTEX_COLOR_ATTR_ID, colors, indices, attribute_interpolation_option::linear};
	m_triangle_mesh.get_color_attributes().push_back(color_info);

	// Add uv attribute to triangle mesh
	//
	std::vector<vector2f> uvs{vector2f{0.5f, 0.0f}, vector2f{0.0f, 1.0f}, vector2f{1.0f, 1.0f}};
	mesh_attribute_info<vector2f> uv_info{TEXCOORD_ATTR_ID, uvs, indices, attribute_interpolation_option::perspective_correct};
	m_triangle_mesh.get_vector2f_attributes().push_back(uv_info);

	// Setup texture shader
	//
	m_texture_shader.set_texture(&m_texture);
};

void rasterized_color_triangle_app::frame(float const delta_since_last_frame)
{
	// Draw the triangle
	//

	if (m_shader_option == shader_option::color)
	{
		get_pipeline().draw(m_triangle_mesh, m_color_shader, get_target_texture());
	}
	else if (m_shader_option == shader_option::texture)
	{
		get_pipeline().draw(m_triangle_mesh, m_texture_shader, get_target_texture());
	}
}

void rasterized_color_triangle_app::on_key_down(SDL_Keysym const key)
{
	app::on_key_down(key);

	float const moving_speed{0.01f};
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
	else if (key.sym == SDLK_1)
	{
		m_shader_option = shader_option::color;
	}
	else if (key.sym == SDLK_2)
	{
		m_shader_option = shader_option::texture;
	}

	// Update model-view-projection according to camera changes
	update_shader_mvp();
}

void rasterized_color_triangle_app::update_shader_mvp()
{
	matrix4x4 const local_to_world_transform{
		matrix4x4::rotation_around_x_axis(m_triangle_rotation.x) *
		matrix4x4::rotation_around_y_axis(m_triangle_rotation.y) *
		matrix4x4::rotation_around_z_axis(m_triangle_rotation.z) *
		matrix4x4::translation(m_triangle_position.x, m_triangle_position.y, m_triangle_position.z)};

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

	if (m_shader_option == shader_option::color)
	{
		m_color_shader.set_mvp_matrix(local_to_clip_transform);
	}
	else
	{
		m_texture_shader.set_mvp_matrix(local_to_clip_transform);
	}
}

int main(int argc, char* argv[])
{
	return rasterized_color_triangle_app{640, 480}.start();
}

