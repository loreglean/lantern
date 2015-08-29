#ifndef LANTERN_COLOR_SHADER_H
#define LANTERN_COLOR_SHADER_H

#include <vector>
#include "shader_bind_point_info.h"
#include "color.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "mesh_attribute_info.h"

namespace lantern
{
	/** Basic color shader
	* @ingroup Shaders
	*/
	class color_shader final
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

		/** Gets info about vector3ff bind points required by shader
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

	private:
		/** Movel-view-projection matrix */
		matrix4x4f m_mvp;

		/** Color bind point, contains interpolated color value */
		color m_color;
	};

	inline void color_shader::set_mvp_matrix(matrix4x4f const& mvp)
	{
		m_mvp = mvp;
	}

	inline vector4f color_shader::process_vertex(vector4f const& vertex)
	{
		return vertex * m_mvp;
	}

	inline color color_shader::process_pixel(vector2ui const& pixel)
	{
		// Just return interpolated color value
		return m_color;
	}

	inline std::vector<shader_bind_point_info<color>> color_shader::get_color_bind_points()
	{
		return std::vector<shader_bind_point_info<color>>{
			shader_bind_point_info<color> { COLOR_ATTR_ID, &m_color }};
	}

	inline std::vector<shader_bind_point_info<float>> color_shader::get_float_bind_points()
	{
		return std::vector<shader_bind_point_info<float>>{};
	}

	inline std::vector<shader_bind_point_info<vector2f>> color_shader::get_vector2f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector2f>>{};
	}

	inline std::vector<shader_bind_point_info<vector3f>> color_shader::get_vector3f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector3f>>{};
	}
}

#endif // LANTERN_COLOR_SHADER_H
