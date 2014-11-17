#ifndef LANTERN_MESH_H
#define LANTERN_MESH_H

#include <vector>
#include "vector3.h"

namespace lantern
{
	class face final
	{
	public:
		unsigned int vertex_index0;
		unsigned int vertex_index1;
		unsigned int vertex_index2;

		unsigned int normal_index0;
		unsigned int normal_index1;
		unsigned int normal_index2;

		unsigned int uv_index0;
		unsigned int uv_index1;
		unsigned int uv_index2;
	};

	class mesh final
	{
	public:
		mesh(
			std::vector<vector3> const& vertices,
			std::vector<vector3> const& uvs,
			std::vector<vector3> const& normals,
			std::vector<face> const& faces);

		std::vector<vector3> const& get_vertices() const;
		std::vector<vector3> const& get_uvs() const;
		std::vector<vector3> const& get_normals() const;
		std::vector<face> const& get_faces() const;

	private:
		std::vector<vector3> const m_vertices;
		std::vector<vector3> const m_uvs;
		std::vector<vector3> const m_normals;
		std::vector<face> const m_faces;
	};
}

#endif // LANTERN_MESH_H
