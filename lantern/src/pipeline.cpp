#include <limits>
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

void pipeline::bind_vertices(std::vector<vector3> const* vertices)
{
	m_vertices = vertices;
}

void pipeline::bind_indices(std::vector<unsigned int> const* indices)
{
	m_indices = indices;
}

void pipeline::bind_color_attribute(mesh_attribute_info<color> const* attr_info, color* bind_point)
{
	m_color_attributes.push_back(binded_mesh_attribute_info<color>{attr_info, bind_point});
}

void pipeline::bind_vector2_attribute(mesh_attribute_info<vector2f> const* attr_info, vector2f* bind_point)
{
	m_vector2_attributes.push_back(binded_mesh_attribute_info<vector2f>{attr_info, bind_point});
}
void pipeline::bind_vector3_attribute(mesh_attribute_info<vector3> const* attr_info, vector3* bind_point)
{
	m_vector3_attributes.push_back(binded_mesh_attribute_info<vector3>{attr_info, bind_point});
}

void pipeline::bind_shader(shader* s)
{
	m_shader = s;
}

void pipeline::reset_state()
{
	m_vertices = nullptr;
	m_indices = nullptr;

	m_color_attributes.clear();
	m_vector2_attributes.clear();
	m_vector3_attributes.clear();

	m_shader = nullptr;
}

void pipeline::draw(bitmap_painter& painter)
{
	size_t vertices_count{m_vertices->size()};

	// Reserve memory immediately to not to waste time during push_back
	//
	std::vector<vector4> transformed_vertices;
	transformed_vertices.reserve(vertices_count);

	for (size_t i{0}; i < vertices_count; ++i)
	{
		vector3 const& v{m_vertices->at(i)};
		vector4 v_transformed{m_shader->process_vertex(vector4{v.x, v.y, v.z, 1.0f})};

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

	for (size_t i{0}; i < vertices_count; ++i)
	{
		vector4& v = transformed_vertices.at(i);

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

	size_t color_attributes_size{m_color_attributes.size()};

	size_t indices_count{m_indices->size()};
	for (size_t i{0}; i < indices_count; i += 3)
	{
		unsigned int const index0{m_indices->at(i + 0)};
		unsigned int const index1{m_indices->at(i + 1)};
		unsigned int const index2{m_indices->at(i + 2)};
		vector4 const& v0{transformed_vertices.at(index0)};
		vector4 const& v1{transformed_vertices.at(index1)};
		vector4 const& v2{transformed_vertices.at(index2)};
		bool const v0_clipped{is_vector4_marked_as_clipped(v0)};
		bool const v1_clipped{is_vector4_marked_as_clipped(v1)};
		bool const v2_clipped{is_vector4_marked_as_clipped(v2)};

		// TODO: rasterization
		// Draws only triangle vertices for now and processes only color attributes just for simplicity
		// Going to be rewritten anyway
		//

		if (!v0_clipped)
		{
			for (size_t j{0}; j < color_attributes_size; ++j)
			{
				binded_mesh_attribute_info<color> const& binded_attribute_info = m_color_attributes.at(j);

				unsigned int const attribute_value_index{binded_attribute_info.info->get_indices().at(index0)};
				(*binded_attribute_info.bind_point) = binded_attribute_info.info->get_data().at(attribute_value_index);
			}

			painter.draw_pixel(
					vector2ui{static_cast<unsigned int>(v0.x), static_cast<unsigned int>(v0.y)},
					m_shader->process_pixel(vector3{v0.x, v0.y, v0.z}));
		}

		if (!v1_clipped)
		{
			for (size_t j{0}; j < color_attributes_size; ++j)
			{
				binded_mesh_attribute_info<color> const& binded_attribute_info = m_color_attributes.at(j);

				unsigned int const attribute_value_index{binded_attribute_info.info->get_indices().at(index1)};
				(*binded_attribute_info.bind_point) = binded_attribute_info.info->get_data().at(attribute_value_index);
			}

			painter.draw_pixel(
					vector2ui{static_cast<unsigned int>(v1.x), static_cast<unsigned int>(v1.y)},
					m_shader->process_pixel(vector3{v1.x, v1.y, v1.z}));
		}

		if (!v2_clipped)
		{
			for (size_t j{0}; j < color_attributes_size; ++j)
			{
				binded_mesh_attribute_info<color> const& binded_attribute_info =m_color_attributes.at(j);

				unsigned int const attribute_value_index{binded_attribute_info.info->get_indices().at(index2)};
				(*binded_attribute_info.bind_point) = binded_attribute_info.info->get_data().at(attribute_value_index);
			}

			painter.draw_pixel(
					vector2ui{static_cast<unsigned int>(v2.x), static_cast<unsigned int>(v2.y)},
					m_shader->process_pixel(vector3{v2.x, v2.y, v2.z}));
		}
	}
}


