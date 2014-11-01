#ifndef LANTERN_MESH_H
#define LANTERN_MESH_H

#include <vector>
#include "vector3.h"

namespace lantern
{
	class face final
	{
	public:
		unsigned int index1;
		unsigned int index2;
		unsigned int index3;
	};

	class mesh final
	{
	public:
		mesh(std::vector<vector3> const& vertices, std::vector<face> const& faces);

		std::vector<vector3> const& get_vertices() const;
		std::vector<face> const& get_faces() const;

	private:
		std::vector<vector3> const m_vertices;
		std::vector<face> const m_faces;
	};
}

#endif // LANTERN_MESH_H
