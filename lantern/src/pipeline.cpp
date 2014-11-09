#include <cmath>
#include <limits>
#include "matrix4x4.h"
#include "pipeline.h"

using namespace lantern;

static void mark_vector4_as_clipped(vector4& v)
{
	v.w = std::numeric_limits<float>::quiet_NaN();
}

static bool is_vector4_marked_as_clipped(vector4 const& v)
{
	return std::isnan(v.w);
}

void pipeline::draw_mesh(
	std::shared_ptr<mesh const> mesh,
	vector3 const& position,
	vector3 const& rotation,
	camera const& camera,
	bitmap_painter& painter) const
{
	matrix4x4 const local_to_world_transform{
		matrix4x4::rotation_around_x_axis(rotation.x) *
		matrix4x4::rotation_around_y_axis(rotation.y) *
		matrix4x4::rotation_around_z_axis(rotation.z) *
		matrix4x4::translation(position.x, position.y, position.z)};

	matrix4x4 const camera_rotation{
		camera.get_right().x, camera.get_up().x, camera.get_forward().x, 0.0f,
		camera.get_right().y, camera.get_up().y, camera.get_forward().y, 0.0f,
		camera.get_right().z, camera.get_up().z, camera.get_forward().z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};
	matrix4x4 const camera_translation{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-camera.get_position().x, -camera.get_position().y, -camera.get_position().z, 1.0f};
	matrix4x4 const world_to_camera_transform{camera_translation * camera_rotation};

    float const projection_plane_z{1.0f};
	float const near{camera.get_near_plane_z()};
	float const far{camera.get_far_plane_z()};
	float const right{std::tan(camera.get_horizontal_fov() / 2.0f) * projection_plane_z};
	float const left{-right};
	float const top{std::tan(camera.get_vertical_fov() / 2.0f) * projection_plane_z};
	float const bottom{-top};
	matrix4x4 const camera_to_homogeneous_clip{
		2.0f * projection_plane_z / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f * projection_plane_z / (top - bottom), 0.0f, 0.0f,
		(left + right) / (left - right), (bottom + top) / (bottom - top), (far + near) / (far - near), 1.0f,
		0.0f, 0.0f, -2.0f * near * far / (far - near), 0.0f};

	matrix4x4 const local_to_clip_transform{
		local_to_world_transform * world_to_camera_transform * camera_to_homogeneous_clip};

	std::vector<vector4> transformed_vertices;
	for (vector3 const& v : mesh->get_vertices())
	{
		vector4 v_transformed{vector4{v.x, v.y, v.z, 1.0f} * local_to_clip_transform};

		if ((v_transformed.x > v_transformed.w) || (v_transformed.x < -v_transformed.w))
		{
			mark_vector4_as_clipped(v_transformed);
		}
		else if ((v_transformed.y > v_transformed.w) || (v_transformed.y < -v_transformed.w))
		{
			mark_vector4_as_clipped(v_transformed);
		}
		else if ((v_transformed.z > v_transformed.w) || (v_transformed.z < -v_transformed.w))
		{
			mark_vector4_as_clipped(v_transformed);
		}

		transformed_vertices.push_back(v_transformed);
	}

    float const width{static_cast<float>(painter.get_bitmap_width())};
    float const height{static_cast<float>(painter.get_bitmap_height())};
    matrix4x4 const ndc_to_screen{
            width / 2.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -height / 2.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            width / 2.0f, height / 2.0f, 0.0f, 1.0f};

	for (vector4& v : transformed_vertices)
	{
		if (is_vector4_marked_as_clipped(v))
		{
			continue;
		}

		float const w_reciprocal{1.0f / v.w};

		v.x *= w_reciprocal;
		v.y *= w_reciprocal;
		v.z *= w_reciprocal;
		v.w = 1.0f;

        v = v * ndc_to_screen;
	}

	for (face const& f : mesh->get_faces())
	{
		vector4 const& v1{transformed_vertices[f.index1]};
		vector4 const& v2{transformed_vertices[f.index2]};
		vector4 const& v3{transformed_vertices[f.index3]};

		bool const v1_clipped{is_vector4_marked_as_clipped(v1)};
		bool const v2_clipped{is_vector4_marked_as_clipped(v2)};
		bool const v3_clipped{is_vector4_marked_as_clipped(v3)};

		if (!v1_clipped && !v2_clipped)
		{
			painter.draw_line(
				point2d{static_cast<unsigned int>(v1.x), static_cast<unsigned int>(v1.y)},
				point2d{static_cast<unsigned int>(v2.x), static_cast<unsigned int>(v2.y)},
				color{255, 255, 255});
		}

		if (!v3_clipped && !v2_clipped)
		{
			painter.draw_line(
				point2d{static_cast<unsigned int>(v2.x), static_cast<unsigned int>(v2.y)},
				point2d{static_cast<unsigned int>(v3.x), static_cast<unsigned int>(v3.y)},
				color{255, 255, 255});
		}

		if (!v1_clipped && !v3_clipped)
		{
			painter.draw_line(
				point2d{static_cast<unsigned int>(v3.x), static_cast<unsigned int>(v3.y)},
				point2d{static_cast<unsigned int>(v1.x), static_cast<unsigned int>(v1.y)},
				color{255, 255, 255});
		}
	}
}

void pipeline::draw_scene(scene const& scene, camera const& camera, bitmap_painter& painter) const
{
	for (scene_object const& obj : scene.get_objects())
	{
		draw_mesh(obj.get_mesh(), obj.get_world_position(), obj.get_world_rotation(), camera, painter);
	}
}
