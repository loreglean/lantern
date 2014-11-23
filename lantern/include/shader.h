#ifndef LANTERN_SHADER_H
#define LANTERN_SHADER_H

#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "color.h"

namespace lantern
{
	/** Class representing shading algorithm.
	*	It should not be treated as something "running on GPU" despite of common notation.
	*	Its purpose is to move shading algorithm into separate object.
	*
	*	Attributes values (like vertex normal, or interpolated color) are filled via bind points.
	*	Bind point is just an address of variable for pipeline to put value into.
	*	They can be registered via pipeline::register_<attr-type>_attribute method.
	*/
	class shader
	{
	public:
		/** Processes vertex. Output vertex must be in homogeneous clip space.
		*/
		virtual vector4 process_vertex(vector4 const& vertex) = 0;

		/** Processes pixel. Output is final pixel color.
		*/
		virtual color process_pixel(vector3 const& pixel) = 0;
	};

	// Basic color shader
	//

	class color_shader : public shader
	{
	public:
		/** Sets model-view-projection matrix to use.
		*/
		void set_mvp_matrix(matrix4x4 const& mvp);

		/** Gets color attribute bind point.
		*/
		color* get_color_attr_bind_point();

		virtual vector4 process_vertex(vector4 const& vertex) override;
		virtual color process_pixel(vector3 const& pixel) override;

	private:
		matrix4x4 m_mvp;
		color m_color;
	};

	// Inline due to huge amount of invocations
	//

	inline vector4 color_shader::process_vertex(vector4 const& vertex)
	{
		return vertex * m_mvp;
	}

	inline color color_shader::process_pixel(vector3 const& pixel)
	{
		return m_color;
	}
}

#endif // LANTERN_SHADER_H
