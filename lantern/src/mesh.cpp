#include "mesh.h"

using namespace lantern;

#if _MSC_VER
mesh::mesh(std::vector<vector3> const& vertices, std::vector<face> const& faces)
{
	m_vertices.assign(vertices.begin(), vertices.end());
	m_faces.assign(faces.begin(), faces.end());
}
#else
mesh::mesh(std::vector<vector3> const& vertices, std::vector<face> const& faces)
	: m_vertices{vertices}, m_faces{faces}
{

}
#endif

std::vector<vector3> const& mesh::get_vertices() const
{
	return m_vertices;
}

std::vector<face> const& mesh::get_faces() const
{
	return m_faces;
}
