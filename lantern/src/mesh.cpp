#include "mesh.h"

using namespace lantern;

mesh::mesh(std::vector<vector3> vertices, std::vector<unsigned int> indices)
	: m_vertices(vertices), m_indices(indices)
{

}

std::vector<vector3> const& mesh::get_vertices() const
{
	return m_vertices;
}

std::vector<unsigned int> const& mesh::get_indices() const
{
	return m_indices;
}

std::vector<mesh_attribute_info<color>> const& mesh::get_color_attributes_storage() const
{
	return m_color_attributes;
}

std::vector<mesh_attribute_info<color>>& mesh::get_color_attributes_storage()
{
	return m_color_attributes;
}

std::vector<mesh_attribute_info<vector2f>> const& mesh::get_vector2_attributes_storage() const
{
	return m_vector2_attributes;
}

std::vector<mesh_attribute_info<vector2f>>& mesh::get_vector2_attributes_storage()
{
	return m_vector2_attributes;
}

std::vector<mesh_attribute_info<vector3>> const& mesh::get_vector3_attributes_storage() const
{
	return m_vector3_attributes;
}

std::vector<mesh_attribute_info<vector3>>& mesh::get_vector3_attributes_storage()
{
	return m_vector3_attributes;
}
