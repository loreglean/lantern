#ifndef LANTERN_MESH_H
#define LANTERN_MESH_H

#include "mesh_attribute_info.h"
#include "vector2.h"
#include "vector3.h"
#include "color.h"

namespace lantern
{
	/** Class representing mesh.
	*	It can be viewed as a container that holds vertices, attributes and their indices.
	*/
	class mesh final
	{
	public:
		mesh(std::vector<vector3> vertices, std::vector<unsigned int> indices);

		/** Gets mesh vertices.
		*
		*/
		std::vector<vector3> const& get_vertices() const;

		/** Get mesh indices.
		*	Used to save memory avoiding vertices duplication.
		*/
		std::vector<unsigned int> const& get_indices() const;

		/** Gets const vector containing list of color attributes.
		*
		*/
		std::vector<mesh_attribute_info<color>> const& get_color_attributes_storage() const;

		/** Gets vector containing list of color attributes.
		*
		*/
		std::vector<mesh_attribute_info<color>>& get_color_attributes_storage();

		/** Gets const vector containing list of vector2 attributes.
		*
		*/
		std::vector<mesh_attribute_info<vector2f>> const& get_vector2_attributes_storage() const;

		/** Gets vector containing list of vector2 attributes.
		*
		*/
		std::vector<mesh_attribute_info<vector2f>>& get_vector2_attributes_storage();

		/** Gets const vector containing list of vector3 attributes.
		*
		*/
		std::vector<mesh_attribute_info<vector3>> const& get_vector3_attributes_storage() const;

		/** Gets vector containing list of vector3 attributes.
		*
		*/
		std::vector<mesh_attribute_info<vector3>>& get_vector3_attributes_storage();

	private:
		std::vector<vector3> const m_vertices;
		std::vector<unsigned int> const m_indices;

		std::vector<mesh_attribute_info<color>> m_color_attributes;
		std::vector<mesh_attribute_info<vector2f>> m_vector2_attributes;
		std::vector<mesh_attribute_info<vector3>> m_vector3_attributes;
	};
}

#endif // LANTERN_MESH_H
