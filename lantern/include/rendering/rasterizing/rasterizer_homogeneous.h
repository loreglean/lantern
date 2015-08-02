#ifndef LANTERN_RASTERIZER_HOMOGENEOUS
#define LANTERN_RASTERIZER_HOMOGENEOUS

#include "math_common.h"
#include "rasterizer_common.h"
#include "rasterizer_traversal_common.h"
#include "rendering_options.h"

namespace lantern
{
	/** Rasterizes triangle using current pipeline setup using homogeneous algorithm
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
	void rasterize_homogeneous(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4f const& v0, vector4f const& v1, vector4f const& v2,
		face_culling_option face_culling,
		TShader& shader,
		texture& target_texture,
		binded_mesh_attributes& attributes)
	{
		// p = ax + by + cw
		// [p0 p1 p2] = [a b c] * m
		// [a b c] = [p0 p1 p2] * m_inversed

		matrix3x3f const m{
			v0.x, v1.x, v2.x,
			v0.y, v1.y, v2.y,
			v0.w, v1.w, v2.w};

		// Can also be interpreted as triple product
		float const det = m.det();

		// Check if m is invertible. If it's not, triangle shouldn't be rendered (volume == 0)
		//
		if (std::abs(det) < FLOAT_EPSILON)
		{
			return;
		}

		// Check if it's back facing
		//
		if (face_culling == face_culling_option::counter_clockwise)
		{
			if (det > 0)
			{
				return;
			}
		}
		else
		{
			if (det < 0)
			{
				return;
			}
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

		std::vector<vector3<color>> color_attrs_abc{attributes.color_attributes.size()};
		std::vector<vector3<float>> float_attrs_abc{attributes.float_attributes.size()};
		std::vector<vector3<vector2f>> vector2f_attrs_abc{attributes.vector2f_attributes.size()};
		std::vector<vector3<vector3f>> vector3f_attrs_abc{attributes.vector3f_attributes.size()};

		save_edges_coefficients<color>(
			attributes.color_attributes,
			color_attrs_abc,
			index0, index1, index2,
			m_inversed);

		save_edges_coefficients<float>(
			attributes.float_attributes,
			float_attrs_abc,
			index0, index1, index2,
			m_inversed);

		save_edges_coefficients<vector2f>(
			attributes.vector2f_attributes,
			vector2f_attrs_abc,
			index0, index1, index2,
			m_inversed);

		save_edges_coefficients<vector3f>(
			attributes.vector3f_attributes,
			vector3f_attrs_abc,
			index0, index1, index2,
			m_inversed);

		// Calculate bounding box
		//

		aabb<vector2ui> bounding_box;

		if ((v0.w < FLOAT_EPSILON) || (v1.w < FLOAT_EPSILON) || (v2.w < FLOAT_EPSILON))
		{
			bounding_box.from = vector2ui{0, 0};
			bounding_box.to = vector2ui{
				static_cast<unsigned int>(target_texture.get_width()) - 1,
				static_cast<unsigned int>(target_texture.get_height()) - 1};
		}
		else
		{
			float v0_screen_x{v0.x / v0.w};
			v0_screen_x = clamp(v0_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));

			float v0_screen_y{v0.y / v0.w};
			v0_screen_y = clamp(v0_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));

			float v1_screen_x{v1.x / v1.w};
			v1_screen_x = clamp(v1_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));

			float v1_screen_y{v1.y / v1.w};
			v1_screen_y = clamp(v1_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));

			float v2_screen_x{v2.x / v2.w};
			v2_screen_x = clamp(v2_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));

			float v2_screen_y{v2.y / v2.w};
			v2_screen_y = clamp(v2_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));

			bounding_box.from.x = static_cast<unsigned int>(std::min(std::min(v0_screen_x, v1_screen_x), v2_screen_x));
			bounding_box.from.y = static_cast<unsigned int>(std::min(std::min(v0_screen_y, v1_screen_y), v2_screen_y));
			bounding_box.to.x = static_cast<unsigned int>(std::max(std::max(v0_screen_x, v1_screen_x), v2_screen_x));
			bounding_box.to.y = static_cast<unsigned int>(std::max(std::max(v0_screen_y, v1_screen_y), v2_screen_y));
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
						attributes.color_attributes,
						color_attrs_abc,
						pc,
						w_value);

					set_bind_points_values_from_edge_coefficients<float>(
						attributes.float_attributes,
						float_attrs_abc,
						pc,
						w_value);

					set_bind_points_values_from_edge_coefficients<vector2f>(
						attributes.vector2f_attributes,
						vector2f_attrs_abc,
						pc,
						w_value);

					set_bind_points_values_from_edge_coefficients<vector3f>(
						attributes.vector3f_attributes,
						vector3f_attrs_abc,
						pc,
						w_value);

					target_texture.set_pixel_color(p, shader.process_pixel(p));
				}

				edge0_v += edge0_abc.x;
				edge1_v += edge1_abc.x;
				edge2_v += edge2_abc.x;
				one_div_w_v += one_div_w_abc.x;
			}
		}
	}

	template<typename TAttr>
	void save_edges_coefficients(
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
	void set_bind_points_values_from_edge_coefficients(
		std::vector<binded_mesh_attribute_info<TAttr>>& binds,
		std::vector<vector3<TAttr>> const& coefficients_storage,
		vector2f const& point,
		float const w)
	{
		for (size_t i = 0; i < binds.size(); ++i)
		{
			vector3<TAttr> abc{coefficients_storage.at(i)};
			TAttr const value_div_w = abc.x * point.x + abc.y * point.y + abc.z;

			binded_mesh_attribute_info<TAttr>& binded_attr = binds[i];
			(*binded_attr.bind_point) = value_div_w * w;
		}
	}
}

#endif // LANTERN_RASTERIZER_HOMOGENEOUS