#ifndef LANTERN_RASTERIZER_INVERSED_SLOPE
#define LANTERN_RASTERIZER_INVERSED_SLOPE

#include "math_common.h"
#include "rasterizer_common.h"

namespace lantern
{
	/** Rasterizes triangle using current pipeline setup using inversed slope algorithm
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
	void rasterize_inversed_slope(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4f const& v0, vector4f const& v1, vector4f const& v2,
		TShader& shader,
		texture& target_texture,
		merger const& merger,
		binded_mesh_attributes& attributes)
	{
		// Sort vertices by y-coordinate
		//

		vector4f v0_sorted{v0};
		vector4f v1_sorted{v1};
		vector4f v2_sorted{v2};

		unsigned int index0_sorted{index0};
		unsigned int index1_sorted{index1};
		unsigned int index2_sorted{index2};

		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v0_sorted, v1_sorted);
			std::swap(index0_sorted, index1_sorted);
		}

		if (v2_sorted.y < v1_sorted.y)
		{
			std::swap(v2_sorted, v1_sorted);
			std::swap(index2_sorted, index1_sorted);
		}

		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v1_sorted, v0_sorted);
			std::swap(index1_sorted, index0_sorted);
		}

		// v0 is top vertex
		// v2 is bottom vertex

		// Check if it's a top, bottom or custom triangle
		//

		if (std::abs(v1_sorted.y - v2_sorted.y) < FLOAT_EPSILON)
		{
			// Top triangle
			//

			if (v2_sorted.x < v1_sorted.x)
			{
				std::swap(v2_sorted, v1_sorted);
				std::swap(index2_sorted, index1_sorted);
			}

			rasterize_inverse_slope_top_or_bottom_triangle(
				index0_sorted, index1_sorted, index2_sorted,
				v0_sorted, v1_sorted, v2_sorted,
				0.0f, 0.0f,
				shader,
				target_texture,
				merger,
				attributes);
		}
		else if (std::abs(v0_sorted.y - v1_sorted.y) < FLOAT_EPSILON)
		{
			// Bottom triangle
			//

			if (v1_sorted.x < v0_sorted.x)
			{
				std::swap(v1_sorted, v0_sorted);
				std::swap(index1_sorted, index0_sorted);
			}

			rasterize_inverse_slope_top_or_bottom_triangle(
				index2_sorted, index0_sorted, index1_sorted,
				v2_sorted, v0_sorted, v1_sorted,
				0.0f, 0.0f,
				shader,
				target_texture,
				merger,
				attributes);
		}
		else
		{
			// Line that divides triangle into top and bottom
			line const separator_line{v1_sorted.x, v1_sorted.y, v1_sorted.x + 1.0f, v1_sorted.y};

			// Calculate intersection point
			vector2f const intersection{separator_line.intersection(line{v0_sorted.x, v0_sorted.y, v2_sorted.x, v2_sorted.y})};

			// Distance in pixels from top vertex to intersection vertex and from top vertex to bottom vertex on the same edge
			// We need these because when we interpolate attributes across edge we need to know real edge length,
			// even though we draw top and bottom triangles separately
			//
			float const distance_to_separator_vertex{vector2f{v0_sorted.x, v0_sorted.y}.distance_to(intersection)};
			float const total_edge_with_separator_vertex_length{vector2f{v2_sorted.x, v2_sorted.y}.distance_to(vector2f{v0_sorted.x, v0_sorted.y})};

			// Vertices that lie on separating line
			// We need two different vectors because we do not calculate interpolated values of z and w and use just distance offset
			//
			vector4f separator_vertex_for_top_triangle{intersection.x, intersection.y, v2.z, v2.w};
			vector4f separator_vertex_for_bottom_triangle{intersection.x, intersection.y, v0.z, v0.w};

			// Draw top triangle
			rasterize_inverse_slope_top_or_bottom_triangle(
				index0_sorted, index2_sorted, index1_sorted,
				v0_sorted, separator_vertex_for_top_triangle, v1_sorted,
				total_edge_with_separator_vertex_length - distance_to_separator_vertex, 0.0f,
				shader,
				target_texture,
				merger,
				attributes);

			// Draw bottom triangle
			rasterize_inverse_slope_top_or_bottom_triangle(
				index2_sorted, index0_sorted, index1_sorted,
				v2_sorted, separator_vertex_for_bottom_triangle, v1_sorted,
				distance_to_separator_vertex, 0.0f,
				shader,
				target_texture,
				merger,
				attributes);
		}
	}

	/** Rasterizes triangle using inversed slope algorithm, triangle should be either top or bottom one
	* @param index0 Top (for top triangle) or bottom (for bottom triangle) vertex index
	* @param index1 Horizontal edge first vertex index
	* @param index2 Horizontal edge second vertex index
	* @param v0 First vertex (associated with index0)
	* @param v1 Second vertex (associated with index1)
	* @param v2 Third vertex (associated with index2)
	* @param v0_v1_edge_distance_offset Distance offset on v0-v1 edge (length of edge part which is not included in rasterizing triangle)
	* @param v0_v2_edge_distance_offset Distance offset on v0-v2 edge (length of edge part which is not included in rasterizing triangle)
	* @param shader Shader to use
	* @param target_texture Texture to draw into
	*/
	template<typename TShader>
	void rasterize_inverse_slope_top_or_bottom_triangle(
		unsigned int index0, unsigned int index1, unsigned int index2,
		vector4f v0, vector4f v1, vector4f v2,
		float v0_v1_edge_distance_offset, float v0_v2_edge_distance_offset,
		TShader& shader,
		texture& target_texture,
		merger const& merger,
		binded_mesh_attributes& attributes)
	{
		// Sort vertices on horizontal edge by their x-coordinate
		//
		if (v1.x > v2.x)
		{
			std::swap(v1, v2);
			std::swap(index1, index2);
			std::swap(v0_v1_edge_distance_offset, v0_v2_edge_distance_offset);
		}

		// True = current triangle is a top triangle (v0.y < v1.y, v0.y < v2.y)
		bool const is_top_triangle{v0.y < v1.y};

		// Direction coefficient
		int const y_order_coefficient{is_top_triangle ? 1 : -1};

		// Left edge length
		float const left_total_distance{vector2f{v0.x, v0.y}.distance_to(vector2f{v1.x, v1.y}) + v0_v1_edge_distance_offset};

		// Right edge length
		float const right_total_distance{vector2f{v0.x, v0.y}.distance_to(vector2f{v2.x, v2.y}) + v0_v2_edge_distance_offset};

		// For how many x's we move on the left edge for one y
		float const inversed_slope_left{(v0.x - v1.x) / (v0.y - v1.y) * y_order_coefficient};

		// For how many x's we move on the right edge for one y
		float const inversed_slope_right{(v0.x - v2.x) / (v0.y - v2.y) * y_order_coefficient};

		// Percent of left edge's length we pass when we increase y by one
		float const left_distance_step_normalized{std::sqrt(1.0f + inversed_slope_left * inversed_slope_left) / left_total_distance};

		// Percent of right edge's length we pass when we increase y by one
		float const right_distance_step_normalized{std::sqrt(1.0f + inversed_slope_right * inversed_slope_right) / right_total_distance};

		// Find next pixel center (if v0 is on the center already, move to the next pixel according to the top-left rule)
		float const next_y_pixel_center{is_top_triangle ? get_next_pixel_center_exclusive(v0.y) : get_previous_pixel_center_exclusive(v0.y)};

		// Calculate offset from the top to the next pixel center
		float const next_y_pixel_center_delta{std::abs(next_y_pixel_center - v0.y)};

		if ((next_y_pixel_center < 0.0f) || (next_y_pixel_center > target_texture.get_height()))
		{
			// If first scanline y coordinate is negative - we do not process a triangle
			// Because either it wasn't clipped or it does not occupy any pixel center (along y axis) in a texture
			return;
		}

		// Calculate last pixel y coordinate
		float const last_y_pixel_center{is_top_triangle ? get_previous_pixel_center_exclusive(v1.y) : get_next_pixel_center_inclusive(v1.y)};

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
		float x_left{v0.x + inversed_slope_left * next_y_pixel_center_delta};
		float x_right{v0.x + inversed_slope_right * next_y_pixel_center_delta};

		// Calculate first and last scanlines integer y-coordinates
		int const first_y{static_cast<int>(next_y_pixel_center)};
		int const last_y{static_cast<int>(last_y_pixel_center) + y_order_coefficient};

		// Initialize attributes storage
		//

		size_t color_binds_count{attributes.color_attributes.size()};
		std::vector<color> left_color_attributes(color_binds_count);
		std::vector<color> right_color_attributes(color_binds_count);

		size_t float_binds_count{attributes.float_attributes.size()};
		std::vector<float> left_float_attributes(float_binds_count);
		std::vector<float> right_float_attributes(float_binds_count);

		size_t vector2f_binds_count{attributes.vector2f_attributes.size()};
		std::vector<vector2f> left_vector2f_attributes(vector2f_binds_count);
		std::vector<vector2f> right_vector2f_attributes(vector2f_binds_count);

		size_t vector3f_binds_count{attributes.vector3f_attributes.size()};
		std::vector<vector3f> left_vector3f_attributes(vector3f_binds_count);
		std::vector<vector3f> right_vector3f_attributes(vector3f_binds_count);

		// For each scanline
		//
		for (int y{first_y}; y != last_y; y += y_order_coefficient)
		{
			// Calculate attributes values on scanline endpoints
			//

			save_intermediate_attrs_values<color>(
				attributes.color_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_color_attributes, right_color_attributes,
				v0.w, v1.w, v2.w);

			save_intermediate_attrs_values<float>(
				attributes.float_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_float_attributes, right_float_attributes,
				v0.w, v1.w, v2.w);

			save_intermediate_attrs_values<vector2f>(
				attributes.vector2f_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_vector2f_attributes, right_vector2f_attributes,
				v0.w, v1.w, v2.w);

			save_intermediate_attrs_values<vector3f>(
				attributes.vector3f_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_vector3f_attributes, right_vector3f_attributes,
				v0.w, v1.w, v2.w);

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
			float const left_zview_reciprocal = (1.0f - current_left_distance_normalized) * v0.w + current_left_distance_normalized * v1.w;
			float const right_zview_reciprocal = (1.0f - current_right_distance_normalized) * v0.w + current_right_distance_normalized * v2.w;

			for (int x{first_x}; x <= last_x; ++x)
			{
				// Calculate attributes values on current pixel center
				//

				set_bind_points_values_from_scanline_endpoints<color>(
					attributes.color_attributes,
					left_color_attributes, right_color_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				set_bind_points_values_from_scanline_endpoints<float>(
					attributes.float_attributes,
					left_float_attributes, right_float_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				set_bind_points_values_from_scanline_endpoints<vector2f>(
					attributes.vector2f_attributes,
					left_vector2f_attributes, right_vector2f_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				set_bind_points_values_from_scanline_endpoints<vector3f>(
					attributes.vector3f_attributes,
					left_vector3f_attributes, right_vector3f_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);

				vector2ui pixel{static_cast<unsigned int>(x), static_cast<unsigned int>(y)};
				merger.merge(target_texture, pixel, shader.process_pixel(pixel));

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
	void set_bind_points_values_from_scanline_endpoints(
		std::vector<binded_mesh_attribute_info<TAttr>>& binds,
		std::vector<TAttr> const& left_endpoint_values,
		std::vector<TAttr> const& right_endpoint_values,
		float const scanline_distance_normalized,
		float const zview_reciprocal_left, float const zview_reciprocal_right)
	{
		size_t binds_count{binds.size()};
		for (size_t i{0}; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr>& binded_attr = binds[i];

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

	/** Gets next pixel center exclusively (if we're on 0.5, we move forward anyway)
	* @param f Current position
	* @returns Pixel center coordinate
	*/
	inline float get_next_pixel_center_exclusive(float const f)
	{
		return (std::floor(f + 0.5f) + 0.5f);
	}

	/** Gets next pixel center inclusively (if we're on 0.5, we don't move)
	* @param f Current position
	* @returns Pixel center coordinate
	*/
	inline float get_next_pixel_center_inclusive(float const f)
	{
		return (std::floor(f + (0.5f - FLOAT_EPSILON)) + 0.5f);
	}
	
	/** Gets next previous center exclusively (if we're on 0.5, we move back anyway)
	* @param f Current position
	* @returns Pixel center coordinate
	*/
	inline float get_previous_pixel_center_exclusive(float const f)
	{
		return (std::floor(f - (0.5f + FLOAT_EPSILON)) + 0.5f);
	}
}

#endif