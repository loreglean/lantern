#ifndef LANTERN_PIPELINE_H
#define LANTERN_PIPELINE_H

#include <limits>
#include <algorithm>
#include <stdexcept>
#include "vector4.h"
#include "matrix4x4.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "geometry_utils.h"

namespace lantern
{
	/** Class that holds information about binded attribute: its info and bind point */
	template<typename TAttr>
	class binded_mesh_attribute_info final
	{
	public:
		/** Attribute data */
		mesh_attribute_info<TAttr> const& info;

		/** Address of variable to put interpolated value into */
		TAttr* bind_point;
	};

	/** Face culling options */
	enum class face_culling_option
	{
		/** Visible wind order is clockwise */
		clockwise,

		/** Visible wind order is counter clockwise */
		counter_clockwise
	};

	/** Rasterization algorithms option */
	enum class rasterization_algorithm_option
	{
		/** Rasterization using traversal algorithm with axis-aligned bounding box */
		traversal_aabb,

		/** Rasterization using traversal alghorithm with backtracking */
		traversal_backtracking
	};

	enum class fill_mode_option
	{
		/** Solid triangle rasterization */
		solid,

		/** Wireframe triangle rasterization (edges only) */
		wireframe
	};

	/** Class representing rendering pipeline */
	class pipeline final
	{
	public:
		/** Constructs pipeline with default parameters */
		pipeline();

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

		/** Rasterizes triangle using current pipeline setup using traversal aabb algorithm
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param v0 First triangle vertex
		* @param v1 Second triangle vertex
		* @param v2 Third triangle vertex
		* @param shader Shader to use
		* @param target_texture Texture to draw into
		*/
		template<typename TShader>
		void rasterize_traversal_aabb(
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			vector4 const& v0, vector4 const& v1, vector4 const& v2,
			TShader& shader, texture& target_texture);

		/** Rasterizes triangle using current pipeline setup using traversal backtracking algorithm
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param v0 First triangle vertex
		* @param v1 Second triangle vertex
		* @param v2 Third triangle vertex
		* @param shader Shader to use
		* @param target_texture Texture to draw into
		*/
		template<typename TShader>
		void rasterize_traversal_backtracking(
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			vector4 const& v0, vector4 const& v1, vector4 const& v2,
			TShader& shader, texture& target_texture);

		/** Checks if point lies on edge's positive halfplane, using top-left rule for points on the edge
		* @param edge_equation_value Value of edge equation in the point
		* @param edge_equation_a X-coefficient of edge equation
		* @param edge_equation_b Y-coefficient of edge equation
		* @returns True if point is either inside positive halfplane or it is on a left or a top edge
		*/
		bool is_point_on_positive_halfplane_top_left(
			float const edge_equation_value, float const edge_equation_a, float const edge_equation_b) const;

		/** Sets binds points values basing on barycentric coordinates
		* @param binds List of binds
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param b0 First barycentric coordinate
		* @param b1 Second barycentric coordinate
		* @param b2 Third barycentric coordinate
		*/
		template<typename TAttr>
		void set_bind_points_values_from_barycentric(
			std::vector<binded_mesh_attribute_info<TAttr>>& binds,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			float const b0, float const b1, float const b2);

		/** Current rasterization algorithm */
		rasterization_algorithm_option m_rasterization_algorithm;

		/** Current vertices wind order for culling */
		face_culling_option m_face_culling;

		/** Current fill mode,
		* wireframe mode uses it's own rasterization function and does not depend of m_rasterization_algorithm value */
		fill_mode_option m_fill_mode;

		/** Color bindings */
		std::vector<binded_mesh_attribute_info<color>> m_binded_color_attributes;

		/** Float bindings */
		std::vector<binded_mesh_attribute_info<float>> m_binded_float_attributes;

		/** Vector2f bindings */
		std::vector<binded_mesh_attribute_info<vector2f>> m_binded_vector2f_attributes;

		/** Vector3 bindings */
		std::vector<binded_mesh_attribute_info<vector3>> m_binded_vector3_attributes;

		/** Storage for transformed vertices */
		std::vector<vector4> m_transformed_vertices_storage;
	};

	inline void mark_vector4_as_clipped(vector4& v)
	{
		v.w = std::numeric_limits<float>::quiet_NaN();
	}

	inline bool is_vector4_marked_as_clipped(vector4 const& v)
	{
		return std::isnan(v.w);
	}

	template<typename TShader>
	void pipeline::draw(mesh const& mesh, TShader& shader, texture& target_texture)
	{
		// Prepare bind points for all available types
		//
		bind_attributes(shader.get_color_bind_points(), mesh.get_color_attributes(), m_binded_color_attributes);
		bind_attributes(shader.get_float_bind_points(), mesh.get_float_attributes(), m_binded_float_attributes);
		bind_attributes(shader.get_vector2f_bind_points(), mesh.get_vector2f_attributes(), m_binded_vector2f_attributes);
		bind_attributes(shader.get_vector3_bind_points(), mesh.get_vector3_attributes(), m_binded_vector3_attributes);

		// Change transformed vertices storage capacity if needed and clear it
		//

		std::vector<vector3> const& vertices = mesh.get_vertices();
		size_t vertices_count{vertices.size()};

		if (m_transformed_vertices_storage.capacity() < vertices_count)
		{
			m_transformed_vertices_storage.reserve(vertices_count);
		}

		m_transformed_vertices_storage.clear();

		// Process vertices and clip
		//
		for (size_t i{0}; i < vertices_count; ++i)
		{
			vector3 const& v{vertices.at(i)};
			vector4 v_transformed{shader.process_vertex(vector4{v.x, v.y, v.z, 1.0f})};

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

			m_transformed_vertices_storage.push_back(v_transformed);
		}

		// Transform vertices to screen coordinates
		//

		float const width{static_cast<float>(target_texture.get_width())};
		float const height{static_cast<float>(target_texture.get_height())};
		matrix4x4 const ndc_to_screen{
			(width) / 2.0f, 0.0f, 0.0f, 0.0f,
			0.0f, -(height) / 2.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			(width) / 2.0f, (height) / 2.0f, 0.0f, 1.0f};

		for (size_t i{0}; i < vertices_count; ++i)
		{
			vector4& v = m_transformed_vertices_storage.at(i);

			if (is_vector4_marked_as_clipped(v))
			{
				continue;
			}

			float const w_inversed{1.0f / v.w};
			v.x *= w_inversed;
			v.y *= w_inversed;
			v.z *= w_inversed;
			v.w = 1.0f;

			v = v * ndc_to_screen;
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
			vector4 const& v0 = m_transformed_vertices_storage.at(index0);
			vector4 const& v1 = m_transformed_vertices_storage.at(index1);
			vector4 const& v2 = m_transformed_vertices_storage.at(index2);

			// Clip triangle if at least one vertex is outside of a screen
			// TODO: clipping
			if (is_vector4_marked_as_clipped(v0) ||
				is_vector4_marked_as_clipped(v1) ||
				is_vector4_marked_as_clipped(v2))
			{
				continue;
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
							shader, target_texture);
						break;

					case rasterization_algorithm_option::traversal_backtracking:
						rasterize_traversal_backtracking(
							index0, index1, index2,
							v0, v1, v2,
							shader, target_texture);
						break;
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

	template<typename TShader>
	void pipeline::rasterize_traversal_aabb(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4 const& v0, vector4 const& v1, vector4 const& v2,
		TShader& shader, texture& target_texture)
	{
		// Construct edges equations
		//

		vector2f e0_from, e0_to;
		vector2f e1_from, e1_to;
		vector2f e2_from, e2_to;

		// Order of points depends of wind order
		// Because this order defines edge's normal direction
		//
		if (m_face_culling == face_culling_option::clockwise)
		{
			e0_from = vector2f{v0.x, v0.y};
			e0_to = vector2f{v1.x, v1.y};

			e1_from = vector2f{v1.x, v1.y};
			e1_to = vector2f{v2.x, v2.y};

			e2_from = vector2f{v2.x, v2.y};
			e2_to = vector2f{v0.x, v0.y};
		}
		else
		{
			e0_from = vector2f{v1.x, v1.y};
			e0_to = vector2f{v0.x, v0.y};

			e1_from = vector2f{v2.x, v2.y};
			e1_to = vector2f{v1.x, v1.y};

			e2_from = vector2f{v0.x, v0.y};
			e2_to = vector2f{v2.x, v2.y};
		}

		line edge0{e0_from.x, e0_from.y, e0_to.x, e0_to.y};
		line edge1{e1_from.x, e1_from.y, e1_to.x, e1_to.y};
		line edge2{e2_from.x, e2_from.y, e2_to.x, e2_to.y};

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

		// Construct triangle's bounding box
		aabb<vector2ui> bounding_box{
			vector2ui{
				static_cast<unsigned int>(std::min(std::min(v0.x, v1.x), v2.x)),
				static_cast<unsigned int>(std::min(std::min(v0.y, v1.y), v2.y))},
			vector2ui{
				static_cast<unsigned int>(std::max(std::max(v0.x, v1.x), v2.x)),
				static_cast<unsigned int>(std::max(std::max(v0.y, v1.y), v2.y))}};

		// Iterate over bounding box and check if pixel is inside the triangle
		//
		for (unsigned int y{bounding_box.from.y}; y <= bounding_box.to.y; ++y)
		{
			float const pixel_center_y{static_cast<float>(y) + 0.5f};

			float const first_x_center{static_cast<float>(bounding_box.from.x) + 0.5f};

			float edge0_equation_value{edge0.at(first_x_center, pixel_center_y)};
			float edge1_equation_value{edge1.at(first_x_center, pixel_center_y)};
			float edge2_equation_value{edge2.at(first_x_center, pixel_center_y)};

			for (unsigned int x{bounding_box.from.x}; x <= bounding_box.to.x; ++x)
			{
				float const pixel_center_x{static_cast<float>(x) + 0.5f};

				if (is_point_on_positive_halfplane_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfplane_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfplane_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{triangle_2d_area(v0.x, v0.y, v1.x, v1.y, pixel_center_x, pixel_center_y)};
					float const area12{triangle_2d_area(v1.x, v1.y, v2.x, v2.y, pixel_center_x, pixel_center_y)};

					// Calculate barycentric coordinates
					//
					float const b2{area01 * triangle_area_inversed};
					float const b0{area12 * triangle_area_inversed};
					float const b1{1.0f - b0 - b2};

					// Process different attributes
					//

					set_bind_points_values_from_barycentric<color>(
						m_binded_color_attributes,
						index0, index1, index2,
						b0, b1, b2);

					set_bind_points_values_from_barycentric<float>(
						m_binded_float_attributes,
						index0, index1, index2,
						b0, b1, b2);

					set_bind_points_values_from_barycentric<vector2f>(
						m_binded_vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2);

					set_bind_points_values_from_barycentric<vector3>(
						m_binded_vector3_attributes,
						index0, index1, index2,
						b0, b1, b2);

					// Pass pixel to shader
					//
					vector2ui point_ui{x, y};
					target_texture.set_pixel_color(point_ui, shader.process_pixel(point_ui));
				}

				edge0_equation_value += edge0.a;
				edge1_equation_value += edge1.a;
				edge2_equation_value += edge2.a;
			}
		}
	}

	template<typename TShader>
	void pipeline::rasterize_traversal_backtracking(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4 const& v0, vector4 const& v1, vector4 const& v2,
		TShader& shader, texture& target_texture)
	{
		// Construct edges equations
		//

		vector2f e0_from, e0_to;
		vector2f e1_from, e1_to;
		vector2f e2_from, e2_to;

		// Order of points depends of wind order
		// Because this order defines edge's normal direction
		//
		if (m_face_culling == face_culling_option::clockwise)
		{
			e0_from = vector2f{v0.x, v0.y};
			e0_to = vector2f{v1.x, v1.y};

			e1_from = vector2f{v1.x, v1.y};
			e1_to = vector2f{v2.x, v2.y};

			e2_from = vector2f{v2.x, v2.y};
			e2_to = vector2f{v0.x, v0.y};
		}
		else
		{
			e0_from = vector2f{v1.x, v1.y};
			e0_to = vector2f{v0.x, v0.y};

			e1_from = vector2f{v2.x, v2.y};
			e1_to = vector2f{v1.x, v1.y};

			e2_from = vector2f{v0.x, v0.y};
			e2_to = vector2f{v2.x, v2.y};
		}

		line edge0{e0_from.x, e0_from.y, e0_to.x, e0_to.y};
		line edge1{e1_from.x, e1_from.y, e1_to.x, e1_to.y};
		line edge2{e2_from.x, e2_from.y, e2_to.x, e2_to.y};

		bool const edge0_normal_pointing_right = edge0.a > 0;
		bool const edge1_normal_pointing_right = edge1.a > 0;
		bool const edge2_normal_pointing_right = edge2.a > 0;

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

		vector4 v0_sorted{v0};
		vector4 v1_sorted{v1};
		vector4 v2_sorted{v2};

		// Sort vertices by y-coordinate
		//

		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v0_sorted, v1_sorted);
		}

		if (v2_sorted.y < v1_sorted.y)
		{
			std::swap(v2_sorted, v1_sorted);
		}

		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v1_sorted, v0_sorted);
		}

		// v0 is top vertex
		// v2 is bottom vertex

		vector2ui current_pixel{static_cast<unsigned int>(v0_sorted.x), static_cast<unsigned int>(v0_sorted.y)};
		vector2f current_pixel_center{std::floor(v0_sorted.x) + 0.5f, std::floor(v0_sorted.y) + 0.5f};

		float edge0_equation_value{edge0.at(current_pixel_center.x, current_pixel_center.y)};
		float edge1_equation_value{edge1.at(current_pixel_center.x, current_pixel_center.y)};
		float edge2_equation_value{edge2.at(current_pixel_center.x, current_pixel_center.y)};

		while (current_pixel_center.y <= v2_sorted.y)
		{
			// Backtracking
			//
			while (true)
			{
				if ((edge0_normal_pointing_right && edge0_equation_value < 0) ||
					(edge1_normal_pointing_right && edge1_equation_value < 0) ||
					(edge2_normal_pointing_right && edge2_equation_value < 0))
				{
					break;
				}

				current_pixel.x -= 1;
				current_pixel_center.x -= 1.0f;

				edge0_equation_value -= edge0.a;
				edge1_equation_value -= edge1.a;
				edge2_equation_value -= edge2.a;

			}

			// Moving along the scanline
			while (true)
			{
				if ((!edge0_normal_pointing_right && edge0_equation_value < 0) ||
					(!edge1_normal_pointing_right && edge1_equation_value < 0) ||
					(!edge2_normal_pointing_right && edge2_equation_value < 0))
				{
					break;
				}

				if (is_point_on_positive_halfplane_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfplane_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfplane_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{triangle_2d_area(v0.x, v0.y, v1.x, v1.y, current_pixel_center.x, current_pixel_center.y)};
					float const area12{triangle_2d_area(v1.x, v1.y, v2.x, v2.y, current_pixel_center.x, current_pixel_center.y)};

					// Calculate barycentric coordinates
					//
					float const b2{area01 * triangle_area_inversed};
					float const b0{area12 * triangle_area_inversed};
					float const b1{1.0f - b0 - b2};

					// Process different attributes
					//

					set_bind_points_values_from_barycentric<color>(
						m_binded_color_attributes,
						index0, index1, index2,
						b0, b1, b2);

					set_bind_points_values_from_barycentric<float>(
						m_binded_float_attributes,
						index0, index1, index2,
						b0, b1, b2);

					set_bind_points_values_from_barycentric<vector2f>(
						m_binded_vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2);

					set_bind_points_values_from_barycentric<vector3>(
						m_binded_vector3_attributes,
						index0, index1, index2,
						b0, b1, b2);

					// Pass pixel to shader
					//
					target_texture.set_pixel_color(current_pixel, shader.process_pixel(current_pixel));
				}

				current_pixel.x += 1;
				current_pixel_center.x += 1.0f;

				edge0_equation_value += edge0.a;
				edge1_equation_value += edge1.a;
				edge2_equation_value += edge2.a;
			}

			current_pixel.y += 1;
			current_pixel_center.y += 1.0f;

			edge0_equation_value += edge0.b;
			edge1_equation_value += edge1.b;
			edge2_equation_value += edge2.b;
		}
	}

	inline bool pipeline::is_point_on_positive_halfplane_top_left(
		float const edge_equation_value, float const edge_equation_a, float const edge_equation_b) const
	{
		float const epsilon = 0.0001f;

		// If we are on the edge, use top-left filling rule
		//
		if (std::abs(edge_equation_value) < epsilon)
		{
			// edge_equation_value == 0.0f, thus point is on the edge,
			// and we use top-left rule to decide if point is inside a triangle
			//
			if (std::abs(edge_equation_a) < epsilon)
			{
				// edge.a == 0.0f, thus it's a horizontal edge and is either a top or a bottom one
				//

				// If normal y coordinate is pointing up - we are on the bottom edge
				// Otherwise we are on the top edge
				return edge_equation_b > 0.0f;
			}
			else
			{
				// If normal x coordinate is pointing right - we are on the left edge
				// Otherwise we are on the right edge
				return edge_equation_a > 0.0f;
			}
		}
		else
		{
			// Check if we're on a positive halfplane
			return edge_equation_value > 0.0f;
		}
	}

	template<typename TAttr>
	void pipeline::set_bind_points_values_from_barycentric(
		std::vector<binded_mesh_attribute_info<TAttr>>& binds,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		float const b0, float const b1, float const b2)
	{
		size_t binds_count{binds.size()};
		for (size_t i{0}; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr>& binded_attr = binds[i];
			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();

			TAttr const& value0 = binded_attr_data[binded_attr_indices[index0]];
			TAttr const& value1 = binded_attr_data[binded_attr_indices[index1]];
			TAttr const& value2 = binded_attr_data[binded_attr_indices[index2]];

			TAttr result_value = value0 * b0 + value1 * b1 + value2 * b2;
			(*binded_attr.bind_point) = result_value;
		}
	}
}

#endif // LANTERN_PIPELINE_H
