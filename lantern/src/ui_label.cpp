#include "ui_label.h"

using namespace lantern;

ui_label::ui_label(font& font, texture const& target_texture)
	: m_font(font),
	m_ndc_per_pixel{2.0f / target_texture.get_width(), 2.0f / target_texture.get_height()}
{
	m_shader.set_color(color::WHITE);
}

void ui_label::set_text(std::string const& text)
{
	if (text != m_text)
	{
		m_text = text;
		update_mesh();
	}
}

void ui_label::set_color(color const& color)
{
	m_shader.set_color(color);
}

void ui_label::draw(renderer& pipeline, texture& target_texture)
{
	// Remember if alpha blending was enable
	bool const alpha_blending_was_enabled{pipeline.get_merging_stage().get_alpha_blending_enabled()};

	// We need to enable it
	pipeline.get_merging_stage().set_alpha_blending_enabled(true);

	// Draw each symbol's mesh
	//
	size_t meshes_count{m_meshes.size()};
	for (size_t i{0}; i < meshes_count; ++i)
	{
		m_shader.set_symbol_texture(&m_symbols.at(i)->get_texture());
		pipeline.render_mesh(m_meshes.at(i), m_shader, target_texture);
	}

	// Return alpha blending to the old value
	pipeline.get_merging_stage().set_alpha_blending_enabled(alpha_blending_was_enabled);
}

void ui_label::on_position_changed()
{
	// TODO: just move vertices
	update_mesh();
}

void ui_label::update_mesh()
{
	m_symbols.clear();
	m_meshes.clear();

	// Params for quads texturing
	//
	std::vector<vector2f> const quad_uvs{
		vector2f{0.0f, 0.0f},
		vector2f{0.0f, 1.0f},
		vector2f{1.0f, 1.0f},
		vector2f{1.0f, 0.0f}};
	std::vector<unsigned int> const quad_uvs_indices{1, 0, 3, 2};

	float const quad_z = 0.0f;

	vector2f bottom_left_ndc_pos{m_position};

	for (char const c : m_text)
	{
		mesh char_mesh;

		// Get font's symbol and save it
		//
		symbol const& symbol{m_font.get_symbol(c)};
		m_symbols.push_back(&symbol);

		// Calculate metrics in NDC
		//
		vector2f const quad_size_ndc{symbol.get_size().x * m_ndc_per_pixel.x, symbol.get_size().y * m_ndc_per_pixel.y};
		vector2f const bearing_ndc{symbol.get_bearing().x * m_ndc_per_pixel.x, symbol.get_bearing().y * m_ndc_per_pixel.y};
		float const advance_ndc{symbol.get_advance() * m_ndc_per_pixel.x};
		float const baseline_offset_ndc{quad_size_ndc.y - bearing_ndc.y};

		// Create quad's verices
		//
		vector3f v1{bottom_left_ndc_pos.x + bearing_ndc.x, bottom_left_ndc_pos.y - baseline_offset_ndc, quad_z};
		vector3f v2{bottom_left_ndc_pos.x + bearing_ndc.x, bottom_left_ndc_pos.y - baseline_offset_ndc + quad_size_ndc.y, quad_z};
		vector3f v3{bottom_left_ndc_pos.x + bearing_ndc.x + quad_size_ndc.x, bottom_left_ndc_pos.y - baseline_offset_ndc + quad_size_ndc.y, quad_z};
		vector3f v4{bottom_left_ndc_pos.x + bearing_ndc.x + quad_size_ndc.x, bottom_left_ndc_pos.y - baseline_offset_ndc, quad_z};

		// Move position for next char
		bottom_left_ndc_pos.x = bottom_left_ndc_pos.x + advance_ndc;

		// Initialize mesh
		//

		char_mesh.get_vertices().push_back(v1);
		char_mesh.get_vertices().push_back(v2);
		char_mesh.get_vertices().push_back(v3);
		char_mesh.get_vertices().push_back(v4);

		char_mesh.get_indices().push_back(0);
		char_mesh.get_indices().push_back(2);
		char_mesh.get_indices().push_back(1);

		char_mesh.get_indices().push_back(0);
		char_mesh.get_indices().push_back(3);
		char_mesh.get_indices().push_back(2);

		char_mesh.get_vector2f_attributes().push_back(mesh_attribute_info<vector2f>{TEXCOORD_ATTR_ID, quad_uvs, quad_uvs_indices, attribute_interpolation_option::linear});

		// Save it
		m_meshes.push_back(char_mesh);
	}

}