#ifndef LANTERN_MESH_ATTRIBUTE_INFO
#define LANTERN_MESH_ATTRIBUTE_INFO

#include <string>
#include <vector>

namespace lantern
{
	/** Class representing mesh attribute info, like texcoords, colors, normals etc.
	*	Those values will be interpolated by pipeline and passed to a shader during rendering.
	*/
	template<typename T>
	class mesh_attribute_info final
	{
	public:
		mesh_attribute_info(std::string id, std::vector<T> data, std::vector<unsigned int> indices);

		/** Gets attribute id.
		*
		*/
		std::string const& get_id() const;

		/** Gets attribute data.
		*
		*/
		std::vector<T> const& get_data() const;

		/** Gets attributes indices.
		*	Those values define which data value applies for vertex with corresponding index.
		*/
		std::vector<unsigned int> const& get_indices() const;

	private:
		std::string const m_id;
		std::vector<T> const m_data;
		std::vector<unsigned int> const m_indices;
	};

	template<typename T>
	mesh_attribute_info<T>::mesh_attribute_info(std::string id, std::vector<T> data, std::vector<unsigned int> indices)
			: m_id(id), m_data(data), m_indices(indices)
	{

	}

	template<typename T>
	std::string const& mesh_attribute_info<T>::get_id() const
	{
		return m_id;
	}

	template<typename T>
	std::vector<T> const& mesh_attribute_info<T>::get_data() const
	{
		return m_data;
	}

	template<typename T>
	std::vector<unsigned int> const& mesh_attribute_info<T>::get_indices() const
	{
		return m_indices;
	}
}

#endif
