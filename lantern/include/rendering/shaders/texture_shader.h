#ifndef LANTERN_TEXTURE_SHADER_H
#define LANTERN_TEXTURE_SHADER_H

#include <vector>
#include "shader.h"
#include "color.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "mesh_attribute_info.h"
#include "texture.h"

namespace lantern
{
	/** Texturing shader
	* @ingroup Shaders
	*/
	class texture_shader final
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

		/** Sets model-view-projection matrix to use during vertex processing
		* @param mvp Model-view-projection matrix
		*/
		void set_mvp_matrix(matrix4x4f const& mvp);

		/** Sets texture to use for texturing
		* @param tex Texture to use
		*/
		void set_texture(texture const* tex);

	private:
		/** UV coordinates bind point */
		vector2f m_uv;

		/** Movel-view-projection matrix */
		matrix4x4f m_mvp;

		/** Texture to use */
		texture const* m_texture;
	};

	inline void texture_shader::set_mvp_matrix(matrix4x4f const& mvp)
	{
		m_mvp = mvp;
	}

	inline void texture_shader::set_texture(texture const* tex)
	{
		m_texture = tex;
	}

	inline vector4f texture_shader::process_vertex(vector4f const& vertex)
	{
		return vertex * m_mvp;
	}

	inline color texture_shader::process_pixel(vector2ui const& pixel)
	{
		// No filtration for now, use nearest neighbour
		return m_texture->get_pixel_color(
			vector2ui{
				static_cast<unsigned int>(m_texture->get_width() * m_uv.x),
				static_cast<unsigned int>(m_texture->get_height() * m_uv.y)});
	}

	inline std::vector<shader_bind_point_info<color>> texture_shader::get_color_bind_points()
	{
		return std::vector<shader_bind_point_info<color>>{};
	}

	inline std::vector<shader_bind_point_info<float>> texture_shader::get_float_bind_points()
	{
		return std::vector<shader_bind_point_info<float>>{};
	}

	inline std::vector<shader_bind_point_info<vector2f>> texture_shader::get_vector2f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector2f>>{
			shader_bind_point_info<vector2f> { TEXCOORD_ATTR_ID, &m_uv }};
	}

	inline std::vector<shader_bind_point_info<vector3f>> texture_shader::get_vector3f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector3f>>{};
	}
}

#endif // LANTERN_TEXTURE_SHADER_H
