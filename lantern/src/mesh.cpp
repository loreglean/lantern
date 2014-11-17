#include "mesh.h"

using namespace lantern;

mesh::mesh(
		std::vector<vector3> const& vertices,
		std::vector<vector3> const& uvs,
		std::vector<vector3> const& normals,
		std::vector<face> const& faces)
	: m_vertices(vertices),
	  m_uvs(uvs),
	  m_normals(normals),
	  m_faces(faces)
{

}

std::vector<vector3> const& mesh::get_vertices() const
{
	return m_vertices;
}

std::vector<vector3> const& mesh::get_uvs() const
{
	return m_uvs;
}

std::vector<vector3> const& mesh::get_normals() const
{
	return m_normals;
}

std::vector<face> const& mesh::get_faces() const
{
	return m_faces;
}
