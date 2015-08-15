#ifndef LANTERN_RASTERIZER_TRAVERSAL_ZIGZAG
#define LANTERN_RASTERIZER_TRAVERSAL_ZIGZAG

namespace lantern
{
	/** Rasterizes triangle using current pipeline setup using zigzag traversal algorithm
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
	void rasterize_traversal_zigzag(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4f const& v0, vector4f const& v1, vector4f const& v2,
		face_culling_option face_culling,
		TShader& shader,
		texture& target_texture,
		merger const& merger,
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

		bool const edge0_normal_pointing_right = edge0.a > 0;
		bool const edge1_normal_pointing_right = edge1.a > 0;
		bool const edge2_normal_pointing_right = edge2.a > 0;

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

		// Sort vertices by y-coordinate
		//

		vector4f v0_sorted{v0};
		vector4f v1_sorted{v1};
		vector4f v2_sorted{v2};

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

		bool start_direction_is_right{true};
		bool is_moving_right{true};

		// Index of the pixel we should move at when reach a pixel outside of a triangle from one side
		unsigned int zigzag_x_index{current_pixel.x - 1};

		while (current_pixel_center.y <= v2_sorted.y)
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

					merger.merge(target_texture, current_pixel, shader.process_pixel(current_pixel));
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
}

#endif // LANTERN_RASTERIZER_TRAVERSAL_ZIGZAG