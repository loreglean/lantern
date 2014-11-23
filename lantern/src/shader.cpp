#include "shader.h"

using namespace lantern;

void color_shader::set_mvp_matrix(matrix4x4 const& mvp)
{
	m_mvp = mvp;
}

color* color_shader::get_color_attr_bind_point()
{
	return &m_color;
}
