#ifndef LANTERN_RASTERIZER_TRAVERSAL_AABB
#define LANTERN_RASTERIZER_TRAVERSAL_AABB

#include "rasterizer_common.h"
#include "rasterizer_traversal_common.h"
#include "rendering_options.h"

namespace lantern
{
	/** Rasterizes triangle using current pipeline setup using traversal aabb algorithm
	* @param index0 First triangle vertex index in a mesh
	* @param index1 Second triangle vertex index in a mesh
	* @param index2 Third triangle vertex index in a mesh
	* @param v0 First triangle vertex
	* @param v1 Second triangle vertex
	* @param v2 Third triangle vertex
	* @param shader Shader to use
	* @param target_texture Texture to draw into
	* @param attributes Attributes to interpolate
	*/
	template<typename TShader>
	inline void rasterize_traversal_aabb(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4f const& v0, vector4f const& v1, vector4f const& v2,
		face_culling_option face_culling,
		TShader& shader,
		texture& target_texture,
		binded_mesh_attributes& attributes)
	{
		// Construct edges equations, considering that top left point is origin
		//

		line edge0{v1.x, v1.y, v0.x, v0.y};
		line edge1{v2.x, v2.y, v1.x, v1.y};
		line edge2{v0.x, v0.y, v2.x, v2.y};

		// Order of points depends of wind order
		// Because this order defines edge's normal direction
		//
		if (face_culling == face_culling_option::clockwise)
		{
			edge0.a *= -1.0f;
			edge0.b *= -1.0f;
			edge0.c *= -1.0f;

			edge1.a *= -1.0f;
			edge1.b *= -1.0f;
			edge1.c *= -1.0f;

			edge2.a *= -1.0f;
			edge2.b *= -1.0f;
			edge2.c *= -1.0f;
		}

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

				if (is_point_on_positive_halfspace_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfspace_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfspace_top_left(edge2_equation_value, edge2.a, edge2.b))
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
						attributes.color_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);

					set_bind_points_values_from_barycentric<float>(
						attributes.float_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);

					set_bind_points_values_from_barycentric<vector2f>(
						attributes.vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);

					set_bind_points_values_from_barycentric<vector3f>(
						attributes.vector3f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);

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
}

#endif