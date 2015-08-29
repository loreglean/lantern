#ifndef LANTERN_RASTERIZING_STAGE_H
#define LANTERN_RASTERIZING_STAGE_H

#include <algorithm>
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3x3.h"
#include "texture.h"
#include "mesh_attribute_info.h"
#include "line.h"
#include "aabb.h"
#include "math_common.h"

namespace lantern
{
	/** Rasterization algorithms option
	* @ingroup Rendering
	*/
	enum class rasterization_algorithm_option
	{
		/** Rasterization using traversal algorithm with axis-aligned bounding box */
		traversal_aabb,

		/** Rasterization using traversal algorithm with backtracking */
		traversal_backtracking,

		/** Rasterization using traversal algorithm changing direction on every line */
		traversal_zigzag,

		/** Rasterization using inversed slope algorithm */
		inversed_slope,

		/** Rasterization using homogeneous 2d coordinates algorithm.
		* This option uses perspective corrected interpolation for all attributes despite of what is specified in attribute info */
		homogeneous
	};

	/** Class that holds information about binded attribute: its info and bind point.
	* It's required when we rasterize a triangle: we need to know where put each attribute's interpolated value
	* @ingroup Rendering
	*/
	template<typename TAttr>
	class binded_mesh_attribute_info final
	{
	public:
		/** Attribute data */
		mesh_attribute_info<TAttr> const& info;

		/** Address of variable to put interpolated value into */
		TAttr* bind_point;
	};

	/** Container for all the binds
	* @ingroup Rendering
	*/
	class binded_mesh_attributes final
	{
	public:
		std::vector<binded_mesh_attribute_info<color>> color_attributes;
		std::vector<binded_mesh_attribute_info<float>> float_attributes;
		std::vector<binded_mesh_attribute_info<vector2f>> vector2f_attributes;
		std::vector<binded_mesh_attribute_info<vector3f>> vector3f_attributes;
	};

	/** This rendering stage is responsible for calculating which pixels cover a texture
	* @ingroup Rendering
	*/
	class rasterizing_stage final
	{
	public:
		/** Constructs rasterizing stage with default settings */
		rasterizing_stage();

		/** Sets rasterization algorithm to use
		* @param value Algorithm to use
		*/
		void set_rasterization_algorithm(rasterization_algorithm_option algorithm_option);

		/** Gets rasterization algorithm
		* @returns Current rasterization alogirthm
		*/
		rasterization_algorithm_option get_rasterization_algorithm() const;

		/** Invokes stage
		* @param vertex0 First triangle vertex
		* @param vertex1 Second triangle vertex
		* @param vertex2 Third triangle vertex
		* @param index0 First vertex attribute index
		* @param index1 Second vertex attribute index
		* @param index2 Third vertex attribute index
		* @param shader Shader to use
		* @param binded_attributes Mesh attributes binded to shader
		* @param target_texture Texture polygon will drawn into
		* @param delegate Object to pass results to for further processing
		*/
		template<typename TShader, typename TDelegate>
		void invoke(
			vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			TShader& shader,
			binded_mesh_attributes const& binded_attributes,
			texture& target_texture,
			TDelegate& delegate);

	private:
		
		// Traversal algorithms
		//

		/** Checks if point lies on edge's positive halfspace, using top-left rule for points on the edge
		* @param edge_equation_value Value of edge equation in the point
		* @param edge_equation_a X-coefficient of edge equation
		* @param edge_equation_b Y-coefficient of edge equation
		* @returns True if point is either inside positive halfplane or it is on a left or a top edge
		*/
		bool is_point_on_positive_halfspace_top_left(
			float const edge_equation_value, float const edge_equation_a, float const edge_equation_b);

		/** Sets binds points values basing on barycentric coordinates
		* @param binds List of binds
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param b0 First barycentric coordinate
		* @param b1 Second barycentric coordinate
		* @param b2 Third barycentric coordinate
		* @param z0_view_space_reciprocal 1/z-view for first vertex
		* @param z1_view_space_reciprocal 1/z-view for second vertex
		* @param z1_view_space_reciprocal 1/z-view for third vertex
		*/
		template<typename TAttr>
		void set_bind_points_values_from_barycentric(
			std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			float const b0, float const b1, float const b2,
			float const z0_view_space_reciprocal, float const z1_view_space_reciprocal, float const z2_view_space_reciprocal);

		/** Rasterizes triangle using current pipeline setup using traversal aabb algorithm
		* @param vertex0 First triangle vertex
		* @param vertex1 Second triangle vertex
		* @param vertex2 Third triangle vertex
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param shader Shader to use
		* @param binded_attributes Attributes to interpolate
		* @param target_texture Texture polygon will be drawn into
		* @param delegate Object to pass results to for further processing
		*/
		template<typename TShader, typename TDelegate>
		inline void rasterize_traversal_aabb(
			vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			TShader& shader,
			binded_mesh_attributes const& binded_attributes,
			texture& target_texture,
			TDelegate& delegate);

		/** Rasterizes triangle using current pipeline setup using traversal backtracking algorithm
		* @param vertex0 First triangle vertex
		* @param vertex1 Second triangle vertex
		* @param vertex2 Third triangle vertex
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param shader Shader to use
		* @param binded_attributes Attributes to interpolate
		* @param target_texture Texture polygon will be drawn into
		* @param delegate Object to pass results to for further processing
		*/
		template<typename TShader, typename TDelegate>
		void rasterize_traversal_backtracking(
			vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			TShader& shader,
			binded_mesh_attributes const& binded_attributes,
			texture& target_texture,
			TDelegate& delegate);

		/** Rasterizes triangle using current pipeline setup using zigzag traversal algorithm
		* @param vertex0 First triangle vertex
		* @param vertex1 Second triangle vertex
		* @param vertex2 Third triangle vertex
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param shader Shader to use
		* @param binded_attributes Attributes to interpolate
		* @param target_texture Texture polygon will be drawn into
		* @param delegate Object to pass results to for further processing
		*/
		template<typename TShader, typename TDelegate>
		void rasterize_traversal_zigzag(
			vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			TShader& shader,
			binded_mesh_attributes const& binded_attributes,
			texture& target_texture,
			TDelegate& delegate);

		// Homogeneous algorithm
		//

		template<typename TAttr>
		void save_edges_coefficients(
			std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
			std::vector<vector3<TAttr>>& coefficients_storage,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			matrix3x3f const& vertices_matrix_inversed);

		template<typename TAttr>
		void set_bind_points_values_from_edge_coefficients(
			std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
			std::vector<vector3<TAttr>> const& coefficients_storage,
			vector2f const& point,
			float const w);

		template<typename TShader, typename TDelegate>
		void rasterize_homogeneous(
			vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			TShader& shader,
			binded_mesh_attributes const& binded_attributes,
			texture& target_texture,
			TDelegate& delegate);

		// Inversed slope algorithm
		//

		/** Saves scanline endpoints attributes values into intermediate storage.
		* It saves attribute value divided by view z for attributes with perspective correction instead of just linearly interpolated values
		* @param binds List of binds
		* @param top_vertex_index Index of a top vertex
		* @param left_vertex_index Index of a left vertex
		* @param right_vertex_index Index of a right vertex
		* @param distance_from_top_to_left_normalized Current distance from a top vertex to a left vertex
		* @param distance_from_top_to_right_normalized Current distance from a top vertex to a right vertex
		* @param left_values_storage Storage to put left edge values into
		* @param right_values_storage Storage to put right edge values into
		* @param top_vertex_zview_reciprocal Camera space z for top vertex, required for perspective correction
		* @param left_vertex_zview_reciprocal Camera space z for left vertex, required for perspective correction
		* @param right_vertex_zview_reciprocal Camera space z for right vertex, required for perspective correction
		*/
		template<typename TAttr>
		void save_intermediate_attrs_values(
			std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
			unsigned int const top_vertex_index,
			unsigned int const left_vertex_index,
			unsigned int const right_vertex_index,
			float const distance_from_top_to_left_normalized,
			float const distance_from_top_to_right_normalized,
			std::vector<TAttr>& left_values_storage,
			std::vector<TAttr>& right_values_storage,
			float const top_vertex_zview_reciprocal,
			float const left_vertex_zview_reciprocal,
			float const right_vertex_zview_reciprocal);

		/** Sets bind points values basing on scanline's endpoints and current position
		* @param binds List of binds
		* @param left_endpoint_values List of left endpoint values
		* @param right_endpoint_values List of right endpoint values
		* @param scanline_distance_normalized Current scanline distance from left endpoint to right endpoint
		* @param zview_reciprocal_left Reciprocal of view z for left endpoint, required for attributes with perspective correction
		* @param zview_reciprocal_right Reciprocal of view z for right endpoint, required for attributes with perspective correction
		*/
		template<typename TAttr>
		void set_bind_points_values_from_scanline_endpoints(
			std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
			std::vector<TAttr> const& left_endpoint_values,
			std::vector<TAttr> const& right_endpoint_values,
			float const scanline_distance_normalized,
			float const zview_reciprocal_left, float const zview_reciprocal_right);

		/** Gets next pixel center exclusively (if we're on 0.5, we move forward anyway)
		* @param f Current position
		* @returns Pixel center coordinate
		*/
		inline float get_next_pixel_center_exclusive(float const f);

		/** Gets next pixel center inclusively (if we're on 0.5, we don't move)
		* @param f Current position
		* @returns Pixel center coordinate
		*/
		inline float get_next_pixel_center_inclusive(float const f);

		/** Gets next previous center exclusively (if we're on 0.5, we move back anyway)
		* @param f Current position
		* @returns Pixel center coordinate
		*/
		inline float get_previous_pixel_center_exclusive(float const f);

		/** Rasterizes triangle using inversed slope algorithm
		* @param vertex0 First triangle vertex
		* @param vertex1 Second triangle vertex
		* @param vertex2 Third triangle vertex
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param shader Shader to use
		* @param binded_attributes Attributes to interpolate
		* @param target_texture Texture polygon will be drawn into
		* @param delegate Object to pass results to for further processing
		*/
		template<typename TShader, typename TDelegate>
		void rasterize_inversed_slope(
			vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			TShader& shader,
			binded_mesh_attributes const& binded_attributes,
			texture& target_texture,
			TDelegate& delegate);

		/** Rasterizes triangle using inversed slope algorithm, triangle should be either top or bottom one
		* @param vertex0 First triangle vertex
		* @param vertex1 Second triangle vertex
		* @param vertex2 Third triangle vertex
		* @param index0 First triangle vertex index in a mesh
		* @param index1 Second triangle vertex index in a mesh
		* @param index2 Third triangle vertex index in a mesh
		* @param v0_v1_edge_distance_offset Distance offset on v0-v1 edge (length of edge part which is not included in rasterizing triangle)
		* @param v0_v2_edge_distance_offset Distance offset on v0-v2 edge (length of edge part which is not included in rasterizing triangle)
		* @param shader Shader to use
		* @param binded_attributes Attributes to interpolate
		* @param target_texture Texture to draw into
		* @param delegate Object to pass results to for further processing
		*/
		template<typename TShader, typename TDelegate>
		void rasterize_inverse_slope_top_or_bottom_triangle(
			vector4f& vertex0, vector4f& vertex1, vector4f& vertex2,
			unsigned int index0, unsigned int index1, unsigned int index2,
			float v0_v1_edge_distance_offset, float v0_v2_edge_distance_offset,
			TShader& shader,
			binded_mesh_attributes const& binded_attributes,
			texture& target_texture,
			TDelegate& delegate);

		// Members
		//

		/** Current rasterization algorithm */
		rasterization_algorithm_option m_rasterization_algorithm;
	};

	template<typename TShader, typename TDelegate>
	void rasterizing_stage::invoke(
		vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		TShader& shader,
		binded_mesh_attributes const& binded_attributes,
		texture& target_texture,
		TDelegate& delegate)
	{
		switch (m_rasterization_algorithm)
		{
			case rasterization_algorithm_option::traversal_aabb:
				rasterize_traversal_aabb(
					vertex0, vertex1, vertex2,
					index0, index1, index2,
					shader,
					binded_attributes,
					target_texture,
					delegate);
				break;

			case rasterization_algorithm_option::traversal_backtracking:
				rasterize_traversal_backtracking(
					vertex0, vertex1, vertex2,
					index0, index1, index2,
					shader,
					binded_attributes,
					target_texture,
					delegate);
				break;

			case rasterization_algorithm_option::traversal_zigzag:
				rasterize_traversal_zigzag(
					vertex0, vertex1, vertex2,
					index0, index1, index2,
					shader,
					binded_attributes,
					target_texture,
					delegate);
				break;

			case rasterization_algorithm_option::homogeneous:
				rasterize_homogeneous(
					vertex0, vertex1, vertex2,
					index0, index1, index2,
					shader,
					binded_attributes,
					target_texture,
					delegate);
				break;

			case rasterization_algorithm_option::inversed_slope:
				rasterize_inversed_slope(
					vertex0, vertex1, vertex2,
					index0, index1, index2,
					shader,
					binded_attributes,
					target_texture,
					delegate);
				break;
		}
	}

	// Traversal algorithms
	//

	inline bool rasterizing_stage::is_point_on_positive_halfspace_top_left(
		float const edge_equation_value, float const edge_equation_a, float const edge_equation_b)
	{
		// If we are on the edge, use top-left filling rule
		//
		if (std::abs(edge_equation_value) < FLOAT_EPSILON)
		{
			// edge_equation_value == 0.0f, thus point is on the edge,
			// and we use top-left rule to decide if point is inside a triangle
			//
			if (std::abs(edge_equation_a) < FLOAT_EPSILON)
			{
				// edge.a == 0.0f, thus it's a horizontal edge and is either a top or a bottom one
				//

				// If normal y coordinate is pointing up - we are on the top edge
				// Otherwise we are on the bottom edge
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
	void rasterizing_stage::set_bind_points_values_from_barycentric(
		std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		float const b0, float const b1, float const b2,
		float const z0_view_space_reciprocal, float const z1_view_space_reciprocal, float const z2_view_space_reciprocal)
	{
		size_t binds_count{binds.size()};
		for (size_t i{0}; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr> const& binded_attr = binds[i];
			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();

			TAttr const& value0 = binded_attr_data[binded_attr_indices[index0]];
			TAttr const& value1 = binded_attr_data[binded_attr_indices[index1]];
			TAttr const& value2 = binded_attr_data[binded_attr_indices[index2]];

			if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::linear)
			{
				(*binded_attr.bind_point) = value0 * b0 + value1 * b1 + value2 * b2;
			}
			else if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::perspective_correct)
			{
				TAttr const value0_div_zview = value0 * z0_view_space_reciprocal;
				TAttr const value1_div_zview = value1 * z1_view_space_reciprocal;
				TAttr const value2_div_zview = value2 * z2_view_space_reciprocal;

				float const zview_reciprocal_interpolated = z0_view_space_reciprocal * b0 + z1_view_space_reciprocal * b1 + z2_view_space_reciprocal * b2;
				TAttr value_div_zview_interpolated = value0_div_zview * b0 + value1_div_zview * b1 + value2_div_zview * b2;

				(*binded_attr.bind_point) = value_div_zview_interpolated * (1.0f / zview_reciprocal_interpolated);
			}
		}
	}

	template<typename TShader, typename TDelegate>
	inline void rasterizing_stage::rasterize_traversal_aabb(
		vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		TShader& shader,
		binded_mesh_attributes const& binded_attributes,
		texture& target_texture,
		TDelegate& delegate)
	{
		// Construct edges equations, considering that top left point is origin
		//

		line edge0{vertex1.x, vertex1.y, vertex0.x, vertex0.y};
		line edge1{vertex2.x, vertex2.y, vertex1.x, vertex1.y};
		line edge2{vertex0.x, vertex0.y, vertex2.x, vertex2.y};

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(vertex0.x, vertex0.y, vertex1.x, vertex1.y, vertex2.x, vertex2.y);

		// Construct triangle's bounding box
		aabb<vector2ui> bounding_box{
			vector2ui{
			static_cast<unsigned int>(std::min(std::min(vertex0.x, vertex1.x), vertex2.x)),
			static_cast<unsigned int>(std::min(std::min(vertex0.y, vertex1.y), vertex2.y))},
			vector2ui{
			static_cast<unsigned int>(std::max(std::max(vertex0.x, vertex1.x), vertex2.x)),
			static_cast<unsigned int>(std::max(std::max(vertex0.y, vertex1.y), vertex2.y))}};

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

				if (is_point_on_positive_halfspace_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfspace_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfspace_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{triangle_2d_area(vertex0.x, vertex0.y, vertex1.x, vertex1.y, pixel_center_x, pixel_center_y)};
					float const area12{triangle_2d_area(vertex1.x, vertex1.y, vertex2.x, vertex2.y, pixel_center_x, pixel_center_y)};

					// Calculate barycentric coordinates
					//
					float const b2{area01 * triangle_area_inversed};
					float const b0{area12 * triangle_area_inversed};
					float const b1{1.0f - b0 - b2};

					// Process different attributes
					//

					set_bind_points_values_from_barycentric<color>(
						binded_attributes.color_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<float>(
						binded_attributes.float_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<vector2f>(
						binded_attributes.vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<vector3f>(
						binded_attributes.vector3f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);
					
					vector2ui pixel_coordinates{x, y};
					vector3f sample_point{pixel_center_x, pixel_center_y, 0.0f};
					delegate.process_rasterizing_stage_result(pixel_coordinates, sample_point, shader, target_texture);
				}

				edge0_equation_value += edge0.a;
				edge1_equation_value += edge1.a;
				edge2_equation_value += edge2.a;
			}
		}
	}


	template<typename TShader, typename TDelegate>
	void rasterizing_stage::rasterize_traversal_backtracking(
		vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		TShader& shader,
		binded_mesh_attributes const& binded_attributes,
		texture& target_texture,
		TDelegate& delegate)
	{
		// Construct edges equations, considering that top left point is origin
		//

		line edge0{vertex1.x, vertex1.y, vertex0.x, vertex0.y};
		line edge1{vertex2.x, vertex2.y, vertex1.x, vertex1.y};
		line edge2{vertex0.x, vertex0.y, vertex2.x, vertex2.y};

		bool const edge0_normal_pointing_right = edge0.a > 0;
		bool const edge1_normal_pointing_right = edge1.a > 0;
		bool const edge2_normal_pointing_right = edge2.a > 0;

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(vertex0.x, vertex0.y, vertex1.x, vertex1.y, vertex2.x, vertex2.y);

		vector4f vertex0_sorted{vertex0};
		vector4f vertex1_sorted{vertex1};
		vector4f vertex2_sorted{vertex2};

		// Sort vertices by y-coordinate
		//

		if (vertex1_sorted.y < vertex0_sorted.y)
		{
			std::swap(vertex0_sorted, vertex1_sorted);
		}

		if (vertex2_sorted.y < vertex1_sorted.y)
		{
			std::swap(vertex2_sorted, vertex1_sorted);
		}

		if (vertex1_sorted.y < vertex0_sorted.y)
		{
			std::swap(vertex1_sorted, vertex0_sorted);
		}

		// vertex0 is top vertex
		// vertex2 is bottom vertex

		vector2ui current_pixel{static_cast<unsigned int>(vertex0_sorted.x), static_cast<unsigned int>(vertex0_sorted.y)};
		vector2f current_pixel_center{std::floor(vertex0_sorted.x) + 0.5f, std::floor(vertex0_sorted.y) + 0.5f};

		float edge0_equation_value{edge0.at(current_pixel_center.x, current_pixel_center.y)};
		float edge1_equation_value{edge1.at(current_pixel_center.x, current_pixel_center.y)};
		float edge2_equation_value{edge2.at(current_pixel_center.x, current_pixel_center.y)};

		while (current_pixel_center.y <= vertex2_sorted.y)
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
			//
			while (true)
			{
				if ((!edge0_normal_pointing_right && edge0_equation_value < 0) ||
					(!edge1_normal_pointing_right && edge1_equation_value < 0) ||
					(!edge2_normal_pointing_right && edge2_equation_value < 0))
				{
					break;
				}

				if (is_point_on_positive_halfspace_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfspace_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfspace_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{triangle_2d_area(vertex0.x, vertex0.y, vertex1.x, vertex1.y, current_pixel_center.x, current_pixel_center.y)};
					float const area12{triangle_2d_area(vertex1.x, vertex1.y, vertex2.x, vertex2.y, current_pixel_center.x, current_pixel_center.y)};

					// Calculate barycentric coordinates
					//
					float const b2{area01 * triangle_area_inversed};
					float const b0{area12 * triangle_area_inversed};
					float const b1{1.0f - b0 - b2};

					// Process different attributes
					//

					set_bind_points_values_from_barycentric<color>(
						binded_attributes.color_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<float>(
						binded_attributes.float_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<vector2f>(
						binded_attributes.vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<vector3f>(
						binded_attributes.vector3f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					vector2ui pixel_coordinates{current_pixel.x, current_pixel.y};
					vector3f sample_point{current_pixel_center.x, current_pixel_center.y, 0.0f};
					delegate.process_rasterizing_stage_result(pixel_coordinates, sample_point, shader, target_texture);

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

	template<typename TShader, typename TDelegate>
	void rasterizing_stage::rasterize_traversal_zigzag(
		vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		TShader& shader,
		binded_mesh_attributes const& binded_attributes,
		texture& target_texture,
		TDelegate& delegate)
	{
		// Construct edges equations, considering that top left point is origin
		//

		line edge0{vertex1.x, vertex1.y, vertex0.x, vertex0.y};
		line edge1{vertex2.x, vertex2.y, vertex1.x, vertex1.y};
		line edge2{vertex0.x, vertex0.y, vertex2.x, vertex2.y};

		bool const edge0_normal_pointing_right = edge0.a > 0;
		bool const edge1_normal_pointing_right = edge1.a > 0;
		bool const edge2_normal_pointing_right = edge2.a > 0;

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(vertex0.x, vertex0.y, vertex1.x, vertex1.y, vertex2.x, vertex2.y);

		// Sort vertices by y-coordinate
		//

		vector4f vertex0_sorted{vertex0};
		vector4f vertex1_sorted{vertex1};
		vector4f vertex2_sorted{vertex2};

		if (vertex1_sorted.y < vertex0_sorted.y)
		{
			std::swap(vertex0_sorted, vertex1_sorted);
		}

		if (vertex2_sorted.y < vertex1_sorted.y)
		{
			std::swap(vertex2_sorted, vertex1_sorted);
		}

		if (vertex1_sorted.y < vertex0_sorted.y)
		{
			std::swap(vertex1_sorted, vertex0_sorted);
		}

		// vertex0 is top vertex
		// vertex2 is bottom vertex

		vector2ui current_pixel{static_cast<unsigned int>(vertex0_sorted.x), static_cast<unsigned int>(vertex0_sorted.y)};
		vector2f current_pixel_center{std::floor(vertex0_sorted.x) + 0.5f, std::floor(vertex0_sorted.y) + 0.5f};

		float edge0_equation_value{edge0.at(current_pixel_center.x, current_pixel_center.y)};
		float edge1_equation_value{edge1.at(current_pixel_center.x, current_pixel_center.y)};
		float edge2_equation_value{edge2.at(current_pixel_center.x, current_pixel_center.y)};

		bool start_direction_is_right{true};
		bool is_moving_right{true};

		// Index of the pixel we should move at when reach a pixel outside of a triangle from one side
		unsigned int zigzag_x_index{current_pixel.x - 1};

		while (current_pixel_center.y <= vertex2_sorted.y)
		{
			// Moving along the scanline
			//
			while (true)
			{
				// If we're moving right, we need to check pixel for being outside of a triangle from the right side
				//
				if (is_moving_right)
				{
					// If we're outside of a triangle
					//
					if ((!edge0_normal_pointing_right && edge0_equation_value < 0) ||
						(!edge1_normal_pointing_right && edge1_equation_value < 0) ||
						(!edge2_normal_pointing_right && edge2_equation_value < 0))
					{
						// If we moved only in side, switch direction and move to the first pixel we didn't visit yet
						//
						if (start_direction_is_right)
						{
							is_moving_right = false;

							int const delta = current_pixel.x - zigzag_x_index;

							current_pixel.x -= delta;
							current_pixel_center.x -= delta * 1.0f;

							edge0_equation_value -= delta * edge0.a;
							edge1_equation_value -= delta * edge1.a;
							edge2_equation_value -= delta * edge2.a;

							continue;
						}
						else
						{
							// Otherwise scanline is over
							break;
						}
					}
				}
				else
				{
					// If we're moving left, we need to check pixel for being outside of a triangle from the left side
					//
					if ((edge0_normal_pointing_right && edge0_equation_value < 0) ||
						(edge1_normal_pointing_right && edge1_equation_value < 0) ||
						(edge2_normal_pointing_right && edge2_equation_value < 0))
					{
						// If we moved only in side, switch direction and move to the first pixel we didn't visit yet
						//
						if (!start_direction_is_right)
						{
							is_moving_right = true;

							int const delta = zigzag_x_index - current_pixel.x;

							current_pixel.x += delta;
							current_pixel_center.x += delta * 1.0f;

							edge0_equation_value += delta * edge0.a;
							edge1_equation_value += delta * edge1.a;
							edge2_equation_value += delta * edge2.a;

							continue;
						}
						else
						{
							// Otherwise scanline is over
							break;
						}
					}
				}

				if (is_point_on_positive_halfspace_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfspace_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfspace_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{triangle_2d_area(vertex0.x, vertex0.y, vertex1.x, vertex1.y, current_pixel_center.x, current_pixel_center.y)};
					float const area12{triangle_2d_area(vertex1.x, vertex1.y, vertex2.x, vertex2.y, current_pixel_center.x, current_pixel_center.y)};

					// Calculate barycentric coordinates
					//
					float const b2{area01 * triangle_area_inversed};
					float const b0{area12 * triangle_area_inversed};
					float const b1{1.0f - b0 - b2};

					// Process different attributes
					//

					set_bind_points_values_from_barycentric<color>(
						binded_attributes.color_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<float>(
						binded_attributes.float_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<vector2f>(
						binded_attributes.vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					set_bind_points_values_from_barycentric<vector3f>(
						binded_attributes.vector3f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						vertex0.w, vertex1.w, vertex2.w);

					vector2ui const pixel_coordinates{current_pixel.x, current_pixel.y};
					vector3f const sample_point{current_pixel_center.x, current_pixel_center.y, 0.0f};
					delegate.process_rasterizing_stage_result(pixel_coordinates, sample_point, shader, target_texture);
				}

				if (is_moving_right)
				{
					current_pixel.x += 1;
					current_pixel_center.x += 1.0f;

					edge0_equation_value += edge0.a;
					edge1_equation_value += edge1.a;
					edge2_equation_value += edge2.a;
				}
				else
				{
					current_pixel.x -= 1;
					current_pixel_center.x -= 1.0f;

					edge0_equation_value -= edge0.a;
					edge1_equation_value -= edge1.a;
					edge2_equation_value -= edge2.a;
				}
			}

			current_pixel.y += 1;
			current_pixel_center.y += 1.0f;

			edge0_equation_value += edge0.b;
			edge1_equation_value += edge1.b;
			edge2_equation_value += edge2.b;

			start_direction_is_right = true;
			is_moving_right = true;
			zigzag_x_index = current_pixel.x - 1;
		}
	}

	// Homogeneous algorithm
	//

	template<typename TAttr>
	void rasterizing_stage::save_edges_coefficients(
		std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
		std::vector<vector3<TAttr>>& coefficients_storage,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		matrix3x3f const& vertices_matrix_inversed)
	{
		for (size_t i = 0; i < binds.size(); ++i)
		{
			binded_mesh_attribute_info<TAttr> const& binded_attr = binds[i];
			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();

			TAttr const& value0 = binded_attr_data[binded_attr_indices[index0]];
			TAttr const& value1 = binded_attr_data[binded_attr_indices[index1]];
			TAttr const& value2 = binded_attr_data[binded_attr_indices[index2]];

			coefficients_storage[i] = vector3<TAttr>{value0, value1, value2} *vertices_matrix_inversed;
		}
	}

	template<typename TAttr>
	void rasterizing_stage::set_bind_points_values_from_edge_coefficients(
		std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
		std::vector<vector3<TAttr>> const& coefficients_storage,
		vector2f const& point,
		float const w)
	{
		for (size_t i = 0; i < binds.size(); ++i)
		{
			vector3<TAttr> abc{coefficients_storage.at(i)};
			TAttr const value_div_w = abc.x * point.x + abc.y * point.y + abc.z;

			binded_mesh_attribute_info<TAttr> const& binded_attr = binds[i];
			(*binded_attr.bind_point) = value_div_w * w;
		}
	}

	template<typename TShader, typename TDelegate>
	void rasterizing_stage::rasterize_homogeneous(
		vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		TShader& shader,
		binded_mesh_attributes const& binded_attributes,
		texture& target_texture,
		TDelegate& delegate)
	{
		// p = ax + by + cw
		// [p0 p1 p2] = [a b c] * m
		// [a b c] = [p0 p1 p2] * m_inversed

		matrix3x3f const m{
			vertex0.x, vertex1.x, vertex2.x,
			vertex0.y, vertex1.y, vertex2.y,
			vertex0.w, vertex1.w, vertex2.w};

		// Can also be interpreted as triple product
		float const det = m.det();

		// Check if m is invertible. If it's not, triangle shouldn't be rendered (volume == 0)
		//
		if (std::abs(det) < FLOAT_EPSILON)
		{
			return;
		}

		matrix3x3f const m_inversed{m.inversed_precalc_det(det)};

		// Edges pseudoparameters
		//
		vector3f const edge0_p{1.0f, 0.0f, 0.0f};
		vector3f const edge1_p{0.0f, 1.0f, 0.0f};
		vector3f const edge2_p{0.0f, 0.0f, 1.0f};

		// Edges coefficients
		//
		vector3f const edge0_abc{m_inversed.values[0][0], m_inversed.values[0][1], m_inversed.values[0][2]};
		vector3f const edge1_abc{m_inversed.values[1][0], m_inversed.values[1][1], m_inversed.values[1][2]};
		vector3f const edge2_abc{m_inversed.values[2][0], m_inversed.values[2][1], m_inversed.values[2][2]};

		// 1/w function coefficients
		//
		vector3f const one_div_w_abc{vector3f{1.0f, 1.0f, 1.0f} *m_inversed};

		// Calculate attributes coefficients
		//

		std::vector<vector3<color>> color_attrs_abc{binded_attributes.color_attributes.size()};
		std::vector<vector3<float>> float_attrs_abc{binded_attributes.float_attributes.size()};
		std::vector<vector3<vector2f>> vector2f_attrs_abc{binded_attributes.vector2f_attributes.size()};
		std::vector<vector3<vector3f>> vector3f_attrs_abc{binded_attributes.vector3f_attributes.size()};

		save_edges_coefficients<color>(
			binded_attributes.color_attributes,
			color_attrs_abc,
			index0, index1, index2,
			m_inversed);

		save_edges_coefficients<float>(
			binded_attributes.float_attributes,
			float_attrs_abc,
			index0, index1, index2,
			m_inversed);

		save_edges_coefficients<vector2f>(
			binded_attributes.vector2f_attributes,
			vector2f_attrs_abc,
			index0, index1, index2,
			m_inversed);

		save_edges_coefficients<vector3f>(
			binded_attributes.vector3f_attributes,
			vector3f_attrs_abc,
			index0, index1, index2,
			m_inversed);

		// Calculate bounding box
		//

		aabb<vector2ui> bounding_box;

		if ((vertex0.w < FLOAT_EPSILON) || (vertex1.w < FLOAT_EPSILON) || (vertex2.w < FLOAT_EPSILON))
		{
			bounding_box.from = vector2ui{0, 0};
			bounding_box.to = vector2ui{
				static_cast<unsigned int>(target_texture.get_width()) - 1,
				static_cast<unsigned int>(target_texture.get_height()) - 1};
		}
		else
		{
			float vertex0_screen_x{vertex0.x / vertex0.w};
			vertex0_screen_x = clamp(vertex0_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));

			float vertex0_screen_y{vertex0.y / vertex0.w};
			vertex0_screen_y = clamp(vertex0_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));

			float vertex1_screen_x{vertex1.x / vertex1.w};
			vertex1_screen_x = clamp(vertex1_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));

			float vertex1_screen_y{vertex1.y / vertex1.w};
			vertex1_screen_y = clamp(vertex1_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));

			float vertex2_screen_x{vertex2.x / vertex2.w};
			vertex2_screen_x = clamp(vertex2_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));

			float vertex2_screen_y{vertex2.y / vertex2.w};
			vertex2_screen_y = clamp(vertex2_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));

			bounding_box.from.x = static_cast<unsigned int>(std::min(std::min(vertex0_screen_x, vertex1_screen_x), vertex2_screen_x));
			bounding_box.from.y = static_cast<unsigned int>(std::min(std::min(vertex0_screen_y, vertex1_screen_y), vertex2_screen_y));
			bounding_box.to.x = static_cast<unsigned int>(std::max(std::max(vertex0_screen_x, vertex1_screen_x), vertex2_screen_x));
			bounding_box.to.y = static_cast<unsigned int>(std::max(std::max(vertex0_screen_y, vertex1_screen_y), vertex2_screen_y));
		}

		for (unsigned int y{bounding_box.from.y}; y <= bounding_box.to.y; ++y)
		{
			vector2f const first_pixel_center{bounding_box.from.x + 0.5f, y + 0.5f};

			float edge0_v{edge0_abc.x * first_pixel_center.x + edge0_abc.y * first_pixel_center.y + edge0_abc.z};
			float edge1_v{edge1_abc.x * first_pixel_center.x + edge1_abc.y * first_pixel_center.y + edge1_abc.z};
			float edge2_v{edge2_abc.x * first_pixel_center.x + edge2_abc.y * first_pixel_center.y + edge2_abc.z};

			float one_div_w_v{one_div_w_abc.x * first_pixel_center.x + one_div_w_abc.y * first_pixel_center.y + one_div_w_abc.z};

			for (unsigned int x{bounding_box.from.x}; x <= bounding_box.to.x; ++x)
			{
				vector2ui const p{x, y};
				vector2f const pc{x + 0.5f, y + 0.5f};

				// Because we're interested only in sign of p, and not the actual value, we can multiply instead of dividing, sign will be the same
				//
				if (is_point_on_positive_halfspace_top_left(edge0_v * one_div_w_v, edge0_abc.x, edge0_abc.y) &&
					is_point_on_positive_halfspace_top_left(edge1_v * one_div_w_v, edge1_abc.x, edge1_abc.y) &&
					is_point_on_positive_halfspace_top_left(edge2_v * one_div_w_v, edge2_abc.x, edge2_abc.y))
				{
					// Check if point is behind
					//
					if (one_div_w_v < 0.0f)
					{
						continue;
					}

					float const w_value{1.0f / one_div_w_v};

					set_bind_points_values_from_edge_coefficients<color>(
						binded_attributes.color_attributes,
						color_attrs_abc,
						pc,
						w_value);

					set_bind_points_values_from_edge_coefficients<float>(
						binded_attributes.float_attributes,
						float_attrs_abc,
						pc,
						w_value);

					set_bind_points_values_from_edge_coefficients<vector2f>(
						binded_attributes.vector2f_attributes,
						vector2f_attrs_abc,
						pc,
						w_value);

					set_bind_points_values_from_edge_coefficients<vector3f>(
						binded_attributes.vector3f_attributes,
						vector3f_attrs_abc,
						pc,
						w_value);

					vector2ui const pixel_coordinates{p.x, p.y};
					vector3f const sample_point{pc.x, pc.y, 0.0f};
					delegate.process_rasterizing_stage_result(pixel_coordinates, sample_point, shader, target_texture);
				}

				edge0_v += edge0_abc.x;
				edge1_v += edge1_abc.x;
				edge2_v += edge2_abc.x;
				one_div_w_v += one_div_w_abc.x;
			}
		}
	}

	// Inversed slope algorithm
	//

	/** Saves scanline endpoints attributes values into intermediate storage.
	* It saves attribute value divided by view z for attributes with perspective correction instead of just linearly interpolated values
	* @param binds List of binds
	* @param top_vertex_index Index of a top vertex
	* @param left_vertex_index Index of a left vertex
	* @param right_vertex_index Index of a right vertex
	* @param distance_from_top_to_left_normalized Current distance from a top vertex to a left vertex
	* @param distance_from_top_to_right_normalized Current distance from a top vertex to a right vertex
	* @param left_values_storage Storage to put left edge values into
	* @param right_values_storage Storage to put right edge values into
	* @param top_vertex_zview_reciprocal Camera space z for top vertex, required for perspective correction
	* @param left_vertex_zview_reciprocal Camera space z for left vertex, required for perspective correction
	* @param right_vertex_zview_reciprocal Camera space z for right vertex, required for perspective correction
	*/
	template<typename TAttr>
	void rasterizing_stage::save_intermediate_attrs_values(
		std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
		unsigned int const top_vertex_index,
		unsigned int const left_vertex_index,
		unsigned int const right_vertex_index,
		float const distance_from_top_to_left_normalized,
		float const distance_from_top_to_right_normalized,
		std::vector<TAttr>& left_values_storage,
		std::vector<TAttr>& right_values_storage,
		float const top_vertex_zview_reciprocal,
		float const left_vertex_zview_reciprocal,
		float const right_vertex_zview_reciprocal)
	{
		size_t const binds_count{binds.size()};
		for (size_t i{0}; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr> const& binded_attr = binds[i];
			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();

			TAttr const& top_value = binded_attr_data[binded_attr_indices[top_vertex_index]];
			TAttr const& left_value = binded_attr_data[binded_attr_indices[left_vertex_index]];
			TAttr const& right_value = binded_attr_data[binded_attr_indices[right_vertex_index]];

			if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::linear)
			{
				left_values_storage[i] = top_value * (1.0f - distance_from_top_to_left_normalized) + left_value * distance_from_top_to_left_normalized;
				right_values_storage[i] = top_value * (1.0f - distance_from_top_to_right_normalized) + right_value * distance_from_top_to_right_normalized;
			}
			else
			{
				TAttr const& top_value_div_zview = top_value * top_vertex_zview_reciprocal;
				TAttr const& left_value_div_zview = left_value * left_vertex_zview_reciprocal;
				TAttr const& right_value_div_zview = right_value * right_vertex_zview_reciprocal;

				left_values_storage[i] = top_value_div_zview * (1.0f - distance_from_top_to_left_normalized) + left_value_div_zview * distance_from_top_to_left_normalized;
				right_values_storage[i] = top_value_div_zview * (1.0f - distance_from_top_to_right_normalized) + right_value_div_zview * distance_from_top_to_right_normalized;
			}
		}
	}

	/** Sets bind points values basing on scanline's endpoints and current position
	* @param binds List of binds
	* @param left_endpoint_values List of left endpoint values
	* @param right_endpoint_values List of right endpoint values
	* @param scanline_distance_normalized Current scanline distance from left endpoint to right endpoint
	* @param zview_reciprocal_left Reciprocal of view z for left endpoint, required for attributes with perspective correction
	* @param zview_reciprocal_right Reciprocal of view z for right endpoint, required for attributes with perspective correction
	*/
	template<typename TAttr>
	void rasterizing_stage::set_bind_points_values_from_scanline_endpoints(
		std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
		std::vector<TAttr> const& left_endpoint_values,
		std::vector<TAttr> const& right_endpoint_values,
		float const scanline_distance_normalized,
		float const zview_reciprocal_left, float const zview_reciprocal_right)
	{
		size_t binds_count{binds.size()};
		for (size_t i{0}; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr> const& binded_attr = binds[i];

			if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::linear)
			{
				TAttr const result =
					left_endpoint_values[i] * (1.0f - scanline_distance_normalized) +
					right_endpoint_values[i] * scanline_distance_normalized;

				(*binded_attr.bind_point) = result;
			}
			else
			{
				TAttr const value_div_zview_interpolated = left_endpoint_values[i] * (1.0f - scanline_distance_normalized) + right_endpoint_values[i] * scanline_distance_normalized;

				float const zview_reciprocal_interpolated = (1.0f - scanline_distance_normalized) * zview_reciprocal_left + scanline_distance_normalized * zview_reciprocal_right;

				(*binded_attr.bind_point) = value_div_zview_interpolated * (1.0f / zview_reciprocal_interpolated);
			}

		}
	}

	inline float rasterizing_stage::get_next_pixel_center_exclusive(float const f)
	{
		return (std::floor(f + 0.5f) + 0.5f);
	}

	inline float rasterizing_stage::get_next_pixel_center_inclusive(float const f)
	{
		return (std::floor(f + (0.5f - FLOAT_EPSILON)) + 0.5f);
	}

	inline float rasterizing_stage::get_previous_pixel_center_exclusive(float const f)
	{
		return (std::floor(f - (0.5f + FLOAT_EPSILON)) + 0.5f);
	}

	template<typename TShader, typename TDelegate>
	void rasterizing_stage::rasterize_inversed_slope(
		vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		TShader& shader,
		binded_mesh_attributes const& binded_attributes,
		texture& target_texture,
		TDelegate& delegate)
	{
		// Sort vertices by y-coordinate
		//

		vector4f vertex0_sorted{vertex0};
		vector4f vertex1_sorted{vertex1};
		vector4f vertex2_sorted{vertex2};

		unsigned int index0_sorted{index0};
		unsigned int index1_sorted{index1};
		unsigned int index2_sorted{index2};

		if (vertex1_sorted.y < vertex0_sorted.y)
		{
			std::swap(vertex0_sorted, vertex1_sorted);
			std::swap(index0_sorted, index1_sorted);
		}

		if (vertex2_sorted.y < vertex1_sorted.y)
		{
			std::swap(vertex2_sorted, vertex1_sorted);
			std::swap(index2_sorted, index1_sorted);
		}

		if (vertex1_sorted.y < vertex0_sorted.y)
		{
			std::swap(vertex1_sorted, vertex0_sorted);
			std::swap(index1_sorted, index0_sorted);
		}

		// vertex0 is top vertex
		// vertex2 is bottom vertex

		// Check if it's a top, bottom or custom triangle
		//

		if (std::abs(vertex1_sorted.y - vertex2_sorted.y) < FLOAT_EPSILON)
		{
			// Top triangle
			//

			if (vertex2_sorted.x < vertex1_sorted.x)
			{
				std::swap(vertex2_sorted, vertex1_sorted);
				std::swap(index2_sorted, index1_sorted);
			}

			rasterize_inverse_slope_top_or_bottom_triangle(
				vertex0_sorted, vertex1_sorted, vertex2_sorted,
				index0_sorted, index1_sorted, index2_sorted,
				0.0f, 0.0f,
				shader,
				binded_attributes,
				target_texture,
				delegate);
		}
		else if (std::abs(vertex0_sorted.y - vertex1_sorted.y) < FLOAT_EPSILON)
		{
			// Bottom triangle
			//

			if (vertex1_sorted.x < vertex0_sorted.x)
			{
				std::swap(vertex1_sorted, vertex0_sorted);
				std::swap(index1_sorted, index0_sorted);
			}

			rasterize_inverse_slope_top_or_bottom_triangle(
				vertex2_sorted, vertex0_sorted, vertex1_sorted,
				index2_sorted, index0_sorted, index1_sorted,
				0.0f, 0.0f,
				shader,
				binded_attributes,
				target_texture,
				delegate);
		}
		else
		{
			// Line that divides triangle into top and bottom
			line const separator_line{vertex1_sorted.x, vertex1_sorted.y, vertex1_sorted.x + 1.0f, vertex1_sorted.y};

			// Calculate intersection point
			vector2f const intersection{separator_line.intersection(line{vertex0_sorted.x, vertex0_sorted.y, vertex2_sorted.x, vertex2_sorted.y})};

			// Distance in pixels from top vertex to intersection vertex and from top vertex to bottom vertex on the same edge
			// We need these because when we interpolate attributes across edge we need to know real edge length,
			// even though we draw top and bottom triangles separately
			//
			float const distance_to_separator_vertex{vector2f{vertex0_sorted.x, vertex0_sorted.y}.distance_to(intersection)};
			float const total_edge_with_separator_vertex_length{vector2f{vertex2_sorted.x, vertex2_sorted.y}.distance_to(vector2f{vertex0_sorted.x, vertex0_sorted.y})};

			// Vertices that lie on separating line
			// We need two different vectors because we do not calculate interpolated values of z and w and use just distance offset
			//
			vector4f separator_vertex_for_top_triangle{intersection.x, intersection.y, vertex2.z, vertex2.w};
			vector4f separator_vertex_for_bottom_triangle{intersection.x, intersection.y, vertex0.z, vertex0.w};

			// Draw top triangle
			rasterize_inverse_slope_top_or_bottom_triangle(
				vertex0_sorted, separator_vertex_for_top_triangle, vertex1_sorted,
				index0_sorted, index2_sorted, index1_sorted,
				total_edge_with_separator_vertex_length - distance_to_separator_vertex, 0.0f,
				shader,
				binded_attributes,
				target_texture,
				delegate);

			// Draw bottom triangle
			rasterize_inverse_slope_top_or_bottom_triangle(
				vertex2_sorted, separator_vertex_for_bottom_triangle, vertex1_sorted,
				index2_sorted, index0_sorted, index1_sorted,
				distance_to_separator_vertex, 0.0f,
				shader,
				binded_attributes,
				target_texture,
				delegate);
		}
	}

	template<typename TShader, typename TDelegate>
	void rasterizing_stage::rasterize_inverse_slope_top_or_bottom_triangle(
		vector4f& vertex0, vector4f& vertex1, vector4f& vertex2,
		unsigned int index0, unsigned int index1, unsigned int index2,
		float v0_v1_edge_distance_offset, float v0_v2_edge_distance_offset,
		TShader& shader,
		binded_mesh_attributes const& binded_attributes,
		texture& target_texture,
		TDelegate& delegate)
	{
		// Sort vertices on horizontal edge by their x-coordinate
		//
		if (vertex1.x > vertex2.x)
		{
			std::swap(vertex1, vertex2);
			std::swap(index1, index2);
			std::swap(v0_v1_edge_distance_offset, v0_v2_edge_distance_offset);
		}

		// True = current triangle is a top triangle (vertex0.y < vertex1.y, vertex0.y < vertex2.y)
		bool const is_top_triangle{vertex0.y < vertex1.y};

		// Direction coefficient
		int const y_order_coefficient{is_top_triangle ? 1 : -1};

		// Left edge length
		float const left_total_distance{vector2f{vertex0.x, vertex0.y}.distance_to(vector2f{vertex1.x, vertex1.y}) + v0_v1_edge_distance_offset};

		// Right edge length
		float const right_total_distance{vector2f{vertex0.x, vertex0.y}.distance_to(vector2f{vertex2.x, vertex2.y}) + v0_v2_edge_distance_offset};

		// For how many x's we move on the left edge for one y
		float const inversed_slope_left{(vertex0.x - vertex1.x) / (vertex0.y - vertex1.y) * y_order_coefficient};

		// For how many x's we move on the right edge for one y
		float const inversed_slope_right{(vertex0.x - vertex2.x) / (vertex0.y - vertex2.y) * y_order_coefficient};

		// Percent of left edge's length we pass when we increase y by one
		float const left_distance_step_normalized{std::sqrt(1.0f + inversed_slope_left * inversed_slope_left) / left_total_distance};

		// Percent of right edge's length we pass when we increase y by one
		float const right_distance_step_normalized{std::sqrt(1.0f + inversed_slope_right * inversed_slope_right) / right_total_distance};

		// Find next pixel center (if vertex0 is on the center already, move to the next pixel according to the top-left rule)
		float const next_y_pixel_center{is_top_triangle ? get_next_pixel_center_exclusive(vertex0.y) : get_previous_pixel_center_exclusive(vertex0.y)};

		// Calculate offset from the top to the next pixel center
		float const next_y_pixel_center_delta{std::abs(next_y_pixel_center - vertex0.y)};

		if ((next_y_pixel_center < 0.0f) || (next_y_pixel_center > target_texture.get_height()))
		{
			// If first scanline y coordinate is negative - we do not process a triangle
			// Because either it wasn't clipped or it does not occupy any pixel center (along y axis) in a texture
			return;
		}

		// Calculate last pixel y coordinate
		float const last_y_pixel_center{is_top_triangle ? get_previous_pixel_center_exclusive(vertex1.y) : get_next_pixel_center_inclusive(vertex1.y)};

		if ((last_y_pixel_center < 0.0f) || (last_y_pixel_center > target_texture.get_height()))
		{
			// If first scanline y coordinate is negative - we do not process a triangle
			// Because either it wasn't clipped or it does not occupy any pixel center (along y axis) in a texture
			return;
		}

		// Calculate starting left distance considering offset to the next y pixel center
		float current_left_distance_normalized{left_distance_step_normalized * next_y_pixel_center_delta};

		// Calculate starting right distance considering offset to the next y pixel center
		float current_right_distance_normalized{right_distance_step_normalized * next_y_pixel_center_delta};

		// Calculate first scanline endpoints considering offset the next y pixel center
		//
		float x_left{vertex0.x + inversed_slope_left * next_y_pixel_center_delta};
		float x_right{vertex0.x + inversed_slope_right * next_y_pixel_center_delta};

		// Calculate first and last scanlines integer y-coordinates
		int const first_y{static_cast<int>(next_y_pixel_center)};
		int const last_y{static_cast<int>(last_y_pixel_center) + y_order_coefficient};

		// Initialize attributes storage
		//

		size_t color_binds_count{binded_attributes.color_attributes.size()};
		std::vector<color> left_color_attributes(color_binds_count);
		std::vector<color> right_color_attributes(color_binds_count);

		size_t float_binds_count{binded_attributes.float_attributes.size()};
		std::vector<float> left_float_attributes(float_binds_count);
		std::vector<float> right_float_attributes(float_binds_count);

		size_t vector2f_binds_count{binded_attributes.vector2f_attributes.size()};
		std::vector<vector2f> left_vector2f_attributes(vector2f_binds_count);
		std::vector<vector2f> right_vector2f_attributes(vector2f_binds_count);

		size_t vector3f_binds_count{binded_attributes.vector3f_attributes.size()};
		std::vector<vector3f> left_vector3f_attributes(vector3f_binds_count);
		std::vector<vector3f> right_vector3f_attributes(vector3f_binds_count);

		// For each scanline
		//
		for (int y{first_y}; y != last_y; y += y_order_coefficient)
		{
			// Calculate attributes values on scanline endpoints
			//

			save_intermediate_attrs_values<color>(
				binded_attributes.color_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_color_attributes, right_color_attributes,
				vertex0.w, vertex1.w, vertex2.w);

			save_intermediate_attrs_values<float>(
				binded_attributes.float_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_float_attributes, right_float_attributes,
				vertex0.w, vertex1.w, vertex2.w);

			save_intermediate_attrs_values<vector2f>(
				binded_attributes.vector2f_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_vector2f_attributes, right_vector2f_attributes,
				vertex0.w, vertex1.w, vertex2.w);

			save_intermediate_attrs_values<vector3f>(
				binded_attributes.vector3f_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_vector3f_attributes, right_vector3f_attributes,
				vertex0.w, vertex1.w, vertex2.w);

			// Length of the scanline
			float const total_scanline_distance{x_right - x_left};

			// Length we move for each x step
			float const scanline_step_distance_normalized{1.0f / total_scanline_distance};

			// Calculate first pixel center we start from, according to top-left rule
			float const x_left_next_pixel_center{get_next_pixel_center_inclusive(x_left)};

			if (x_left_next_pixel_center < 0.0f)
			{
				continue;
			}

			// Calculate last pixel center
			float last_x_pixel_center{get_previous_pixel_center_exclusive(x_right)};

			if (last_x_pixel_center < 0.0f)
			{
				continue;
			}

			// Calculate current scanline distance, taking into account starting left pixel offset
			float current_scanline_distance_normalized{(x_left_next_pixel_center - x_left) * scanline_step_distance_normalized};

			int const first_x{static_cast<int>(x_left_next_pixel_center)};
			int const last_x{static_cast<int>(last_x_pixel_center)};

			// Calculate endpoints interpolated z reciprocals
			//
			float const left_zview_reciprocal = (1.0f - current_left_distance_normalized) * vertex0.w + current_left_distance_normalized * vertex1.w;
			float const right_zview_reciprocal = (1.0f - current_right_distance_normalized) * vertex0.w + current_right_distance_normalized * vertex2.w;

			for (int x{first_x}; x <= last_x; ++x)
			{
				// Calculate attributes values on current pixel center
				//

				set_bind_points_values_from_scanline_endpoints<color>(
					binded_attributes.color_attributes,
					left_color_attributes, right_color_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				set_bind_points_values_from_scanline_endpoints<float>(
					binded_attributes.float_attributes,
					left_float_attributes, right_float_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				set_bind_points_values_from_scanline_endpoints<vector2f>(
					binded_attributes.vector2f_attributes,
					left_vector2f_attributes, right_vector2f_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				set_bind_points_values_from_scanline_endpoints<vector3f>(
					binded_attributes.vector3f_attributes,
					left_vector3f_attributes, right_vector3f_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				vector2ui const pixel_coordinates{static_cast<unsigned int>(x), static_cast<unsigned int>(y)};
				vector3f const sample_point{x + 0.5f, y + 0.5f, 0.0f};
				delegate.process_rasterizing_stage_result(pixel_coordinates, sample_point, shader, target_texture);

				current_scanline_distance_normalized += scanline_step_distance_normalized;
			}

			// Move to the next scanline
			//

			x_left += inversed_slope_left;
			x_right += inversed_slope_right;

			current_left_distance_normalized += left_distance_step_normalized;
			current_right_distance_normalized += right_distance_step_normalized;
		}
	}
}

#endif // LANTERN_RASTERIZING_STAGE_H