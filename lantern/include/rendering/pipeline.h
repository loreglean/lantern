#ifndef LANTERN_PIPELINE_H
#define LANTERN_PIPELINE_H

#include <limits>
#include <algorithm>
#include <stdexcept>
#include "vector4.h"
#include "matrix4x4.h"
#include "matrix3x3.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "line.h"
#include "aabb.h"
#include "math_common.h"
#include "merger.h"
#include "rasterizer_traversal_aabb.h"
#include "rasterizer_traversal_backtracking.h"
#include "rasterizer_traversal_zigzag.h"
#include "rasterizer_homogeneous.h"
#include "rasterizer_inversed_slope.h"
#include "rendering_options.h"

// TODO: separate rasterization algorithms into different types?

namespace lantern
{
	/** Class representing rendering pipeline */
	class pipeline final
	{
	public:
		/** Constructs pipeline with default parameters */
		pipeline();

		merger& get_merger();

		/** Sets rasterization algorithm to use
		* @param value Algorithm to use
		*/
		void set_rasterization_algorithm(rasterization_algorithm_option value);

		/** Sets face culling to use
		* @param value Face culling wind order to use
		*/
		void set_face_culling(face_culling_option value);

		/** Sets fill mode to use
		* @param value Fill mode to use during rasterization
		*/
		void set_fill_mode(fill_mode_option value);

		/** Renders mesh to specified texture using specified shader
		* @param mesh Mesh to render
		* @param shader Shader to use
		* @param target_texture Texture to draw into
		*/
		template<typename TShader>
		void draw(mesh const& mesh, TShader& shader, texture& target_texture);

	private:
		/** Binds mesh attributes to shader bind points
		* @param required_bind_points Shader bind points
		* @param available_attributes Mesh attributes
		* @param binded_attributes_storage Storage for bindings
		*/
		template<typename TAttr>
		void bind_attributes(
			std::vector<shader_bind_point_info<TAttr>> const& required_bind_points,
			std::vector<mesh_attribute_info<TAttr>> const& available_attributes,
			std::vector<binded_mesh_attribute_info<TAttr>>& binded_attributes_storage);

		/** Merger */
		merger m_merger;

		/** Current rasterization algorithm */
		rasterization_algorithm_option m_rasterization_algorithm;

		/** Current vertices wind order for culling */
		face_culling_option m_face_culling;

		/** Current fill mode,
		* wireframe mode uses it's own rasterization function and does not depend of m_rasterization_algorithm value */
		fill_mode_option m_fill_mode;

		/** Binded attributes */
		binded_mesh_attributes m_binded_mesh_attributes;

		/** Storage for transformed vertices */
		std::vector<vector4f> m_transformed_vertices_storage;

		/* Storage for clip flags */
		std::vector<bool> m_transformed_vertices_clip_flags_storage;
	};

	template<typename TShader>
	void pipeline::draw(mesh const& mesh, TShader& shader, texture& target_texture)
	{
		// Prepare bind points for all available types
		//
		bind_attributes(shader.get_color_bind_points(), mesh.get_color_attributes(), m_binded_mesh_attributes.color_attributes);
		bind_attributes(shader.get_float_bind_points(), mesh.get_float_attributes(), m_binded_mesh_attributes.float_attributes);
		bind_attributes(shader.get_vector2f_bind_points(), mesh.get_vector2f_attributes(), m_binded_mesh_attributes.vector2f_attributes);
		bind_attributes(shader.get_vector3f_bind_points(), mesh.get_vector3f_attributes(), m_binded_mesh_attributes.vector3f_attributes);

		// Change transformed vertices storages capacity if needed and clear it
		//

		std::vector<vector3f> const& vertices = mesh.get_vertices();
		size_t vertices_count{vertices.size()};

		if (m_transformed_vertices_storage.capacity() < vertices_count)
		{
			m_transformed_vertices_storage.reserve(vertices_count);
		}
		m_transformed_vertices_storage.clear();

		if (m_transformed_vertices_clip_flags_storage.capacity() < vertices_count)
		{
			m_transformed_vertices_clip_flags_storage.reserve(vertices_count);
		}
		m_transformed_vertices_clip_flags_storage.clear();

		// Process vertices and clip
		//
		for (size_t i{0}; i < vertices_count; ++i)
		{
			vector3f const& v{vertices.at(i)};
			vector4f v_transformed{shader.process_vertex(vector4f{v.x, v.y, v.z, 1.0f})};

			bool clipped{false};
			if ((v_transformed.x > v_transformed.w) || (v_transformed.x < -v_transformed.w))
			{
				clipped = true;
			}
			else if ((v_transformed.y > v_transformed.w) || (v_transformed.y < -v_transformed.w))
			{
				clipped = true;
			}
			else if ((v_transformed.z > v_transformed.w) || (v_transformed.z < -v_transformed.w))
			{
				clipped = true;
			}

			m_transformed_vertices_storage.push_back(v_transformed);
			m_transformed_vertices_clip_flags_storage.push_back(clipped);
		}

		// Transform vertices to screen coordinates
		//

		float const width{static_cast<float>(target_texture.get_width())};
		float const height{static_cast<float>(target_texture.get_height())};

		if (m_rasterization_algorithm == rasterization_algorithm_option::homogeneous)
		{
			matrix4x4f const ndc_to_screen{
				(width) / 2.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -(height) / 2.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				(width) / 2.0f, (height) / 2.0f, 0.0f, 1.0f};

			for (size_t i{0}; i < vertices_count; ++i)
			{
				vector4f& v = m_transformed_vertices_storage.at(i);
				v = v * ndc_to_screen;
			}
		}
		else
		{
			for (size_t i{0}; i < vertices_count; ++i)
			{
				if (m_transformed_vertices_clip_flags_storage.at(i) == true)
				{
					continue;
				}

				vector4f& v = m_transformed_vertices_storage.at(i);

				float const w_inversed{1.0f / v.w};

				v.x *= w_inversed;
				v.y *= w_inversed;
				v.z *= w_inversed;

				v.w = w_inversed;

				// NDC to screen
				//
				v.x = v.x * width / 2.0f + width / 2.0f;
				v.y = -v.y * height / 2.0f + height / 2.0f;
			}
		}

		// Rasterization
		//

		std::vector<unsigned int> const& indices = mesh.get_indices();

		size_t indices_count{indices.size()};
		for (size_t i{0}; i < indices_count; i += 3)
		{
			unsigned int const index0{indices.at(i + 0)};
			unsigned int const index1{indices.at(i + 1)};
			unsigned int const index2{indices.at(i + 2)};

			vector4f const& v0 = m_transformed_vertices_storage.at(index0);
			vector4f const& v1 = m_transformed_vertices_storage.at(index1);
			vector4f const& v2 = m_transformed_vertices_storage.at(index2);

			bool const v0_clipped{m_transformed_vertices_clip_flags_storage.at(index0)};
			bool const v1_clipped{m_transformed_vertices_clip_flags_storage.at(index1)};
			bool const v2_clipped{m_transformed_vertices_clip_flags_storage.at(index2)};

			if (m_rasterization_algorithm != rasterization_algorithm_option::homogeneous)
			{
				// Clip triangle if at least one vertex is outside of a screen as a workaround
				// TODO: clipping
				//
				if (v0_clipped || v1_clipped || v2_clipped)
				{
					continue;
				}
			}

			if (m_fill_mode == fill_mode_option::wireframe)
			{
				// TODO
			}
			else
			{
				switch (m_rasterization_algorithm)
				{
					case rasterization_algorithm_option::traversal_aabb:
						rasterize_traversal_aabb(
							index0, index1, index2,
							v0, v1, v2,
							m_face_culling,
							shader,
							target_texture,
							m_merger,
							m_binded_mesh_attributes);
						break;

					case rasterization_algorithm_option::traversal_backtracking:
						rasterize_traversal_backtracking(
							index0, index1, index2,
							v0, v1, v2,
							m_face_culling,
							shader,
							target_texture,
							m_merger,
							m_binded_mesh_attributes);
						break;

					case rasterization_algorithm_option::traversal_zigzag:
						rasterize_traversal_zigzag(
							index0, index1, index2,
							v0, v1, v2,
							m_face_culling,
							shader,
							target_texture,
							m_merger,
							m_binded_mesh_attributes);
						break;

					case rasterization_algorithm_option::inversed_slope:
						rasterize_inversed_slope(
							index0, index1, index2,
							v0, v1, v2,
							shader,
							target_texture,
							m_merger,
							m_binded_mesh_attributes);
						break;

					case rasterization_algorithm_option::homogeneous:
						rasterize_homogeneous(
							index0, index1, index2,
							v0, v1, v2,
							m_face_culling,
							shader,
							target_texture,
							m_merger,
							m_binded_mesh_attributes);
				}
			}
		}
	}

	template<typename TAttr>
	void pipeline::bind_attributes(
		std::vector<shader_bind_point_info<TAttr>> const& required_bind_points,
		std::vector<mesh_attribute_info<TAttr>> const& available_attributes,
		std::vector<binded_mesh_attribute_info<TAttr>>& binded_attributes_storage)
	{
		// Clear the storage
		binded_attributes_storage.clear();

		size_t const bind_points_size{required_bind_points.size()};
		size_t const available_attributes_size{available_attributes.size()};

		for (size_t i{0}; i < bind_points_size; ++i)
		{
			shader_bind_point_info<TAttr> const& bind_point_info = required_bind_points.at(i);

			bool binded{false};

			// Find mesh attribute we can bind to the point
			//
			for (size_t j{0}; j < available_attributes_size; ++j)
			{
				mesh_attribute_info<TAttr> const& attr_info = available_attributes.at(j);
				if (attr_info.get_id() == bind_point_info.attribute_id)
				{
					binded_attributes_storage.push_back(
						binded_mesh_attribute_info<TAttr>{attr_info, bind_point_info.bind_point});

					binded = true;
					break;
				}
			}

			if (!binded)
			{
				throw std::runtime_error("Mesh doesn't contain attribute required by shader");
			}
		}
	};
}

#endif // LANTERN_PIPELINE_H
