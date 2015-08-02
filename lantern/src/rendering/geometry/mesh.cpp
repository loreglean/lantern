#include "mesh.h"

using namespace lantern;

mesh::mesh(std::vector<vector3f> vertices, std::vector<unsigned int> indices)
	: m_vertices(vertices), m_indices(indices)
{

}

std::vector<vector3f> const& mesh::get_vertices() const
{
	return m_vertices;
}

std::vector<unsigned int> const& mesh::get_indices() const
{
	return m_indices;
}

std::vector<mesh_attribute_info<color>>& mesh::get_color_attributes()
{
	return m_color_attributes;
}

std::vector<mesh_attribute_info<color>> const& mesh::get_color_attributes() const
{
	return m_color_attributes;
}

std::vector<mesh_attribute_info<float>>& mesh::get_float_attributes()
{
	return m_float_attributes;
}

std::vector<mesh_attribute_info<float>> const& mesh::get_float_attributes() const
{
	return m_float_attributes;
}

std::vector<mesh_attribute_info<vector2f>>& mesh::get_vector2f_attributes()
{
	return m_vector2f_attributes;
}

std::vector<mesh_attribute_info<vector2f>> const& mesh::get_vector2f_attributes() const
{
	return m_vector2f_attributes;
}

std::vector<mesh_attribute_info<vector3f>>& mesh::get_vector3f_attributes()
{
	return m_vector3f_attributes;
}

std::vector<mesh_attribute_info<vector3f>> const& mesh::get_vector3f_attributes() const
{
	return m_vector3f_attributes;
}