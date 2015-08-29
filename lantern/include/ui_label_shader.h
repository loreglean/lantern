#ifndef LANTERN_UI_LABEL_SHADER_H
#define LANTERN_UI_LABEL_SHADER_H

#include <vector>
#include "shader_bind_point_info.h"
#include "color.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "mesh_attribute_info.h"
#include "texture.h"

namespace lantern
{
	/** Shader for font's symbol rendering, used by ui_label.
	* It doesn't make any vertex transformations - assume that there're already in NDC coordinate space
	* @ingroup Shaders
	*/
	class ui_label_shader final
	{
	public:
		/** Gets info about color bind points required by shader
		* @returns Required color bind points
		*/
		std::vector<shader_bind_point_info<color>> get_color_bind_points();

		/** Gets info about float bind points required by shader
		* @returns Required float bind points
		*/
		std::vector<shader_bind_point_info<float>> get_float_bind_points();

		/** Gets info about vector2f bind points required by shader
		* @returns Required vector2f bind points
		*/
		std::vector<shader_bind_point_info<vector2f>> get_vector2f_bind_points();

		/** Gets info about vector3f bind points required by shader
		* @returns Required vector3f bind points
		*/
		std::vector<shader_bind_point_info<vector3f>> get_vector3f_bind_points();

		/** Processes vertex
		* @param vertex Vertex in local space
		* @returns Processed vertex in homogeneous clip space
		*/
		vector4f process_vertex(vector4f const& vertex);

		/** Processes pixel
		* @param pixel Pixel coordinates on screen
		* @returns Final pixel color
		*/
		color process_pixel(vector2ui const& pixel);

		/** Sets symbols texture
		* @param symbol_texture Symbol's texture to use
		*/
		void set_symbol_texture(texture const* symbol_texture);

		/** Sets color to use for rendering
		* @param color Symbol's color
		*/
		void set_color(color const& color);

	private:
		/** UV coordinates bind point */
		vector2f m_uv;

		/** Symbol texture to use */
		texture const* m_symbol_texture;

		/** Color to render symbols with */
		color m_color;
	};

	inline std::vector<shader_bind_point_info<color>> ui_label_shader::get_color_bind_points()
	{
		return std::vector<shader_bind_point_info<color>>{};
	}

	inline std::vector<shader_bind_point_info<float>> ui_label_shader::get_float_bind_points()
	{
		return std::vector<shader_bind_point_info<float>>{};
	}

	inline std::vector<shader_bind_point_info<vector2f>> ui_label_shader::get_vector2f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector2f>>{
			shader_bind_point_info<vector2f> { TEXCOORD_ATTR_ID, &m_uv }};
	}

	inline std::vector<shader_bind_point_info<vector3f>> ui_label_shader::get_vector3f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector3f>>{};
	}

	inline vector4f ui_label_shader::process_vertex(vector4f const& vertex)
	{
		return vertex;
	}

	inline color ui_label_shader::process_pixel(vector2ui const& pixel)
	{
		color symbol_color = m_symbol_texture->get_pixel_color(
			vector2ui{
			static_cast<unsigned int>(m_symbol_texture->get_width() * m_uv.x),
			static_cast<unsigned int>(m_symbol_texture->get_height() * m_uv.y)});

		// Copy all the channels except for alpha from required color
		//
		symbol_color.r = m_color.r;
		symbol_color.g = m_color.g;
		symbol_color.b = m_color.b;

		return symbol_color;
	}

	inline void ui_label_shader::set_symbol_texture(texture const* tex)
	{
		m_symbol_texture = tex;
	}

	inline void ui_label_shader::set_color(color const& color)
	{
		m_color = color;
	}
}

#endif // LANTERN_UI_LABEL_SHADER_H
