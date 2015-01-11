#ifndef LANTERN_MESH_ATTRIBUTE_INFO
#define LANTERN_MESH_ATTRIBUTE_INFO

#include <vector>

namespace lantern
{
	/** Holds all the information associated with mesh attribute */
	template<typename TAttr>
	class mesh_attribute_info final
	{
	public:
		/** Constructs attribute info
		* @param attribute_id Attribute ID, should be unique
		* @param data Attribute data
		* @param indices Attribute indices
		*/
		mesh_attribute_info(
			unsigned int const attribute_id,
			std::vector<TAttr> const data,
			std::vector<unsigned int> const indices);

		/** Gets attribute ID
		* @returns Attribute ID value
		*/
		unsigned int get_id() const;

		/** Gets attribute data
		* @returns Attribute data
		*/
		std::vector<TAttr> const& get_data() const;

		/** Gets attribute indices
		* @returns Indices
		*/
		std::vector<unsigned int> const& get_indices() const;

	private:
		/** Attribute id */
		unsigned int const m_id;

		/** Attribute data */
		std::vector<TAttr> const m_data;

		/** Attribute indices */
		std::vector<unsigned int> const m_indices;
	};

	// Predefined attributes ids
	//
	unsigned int const VERTEX_COLOR_ATTR_ID = 0;
	unsigned int const TEXCOORD_ATTR_ID = 1;
	unsigned int const NORMAL_ATTR_ID = 2;

	template<typename TAttr>
	mesh_attribute_info<TAttr>::mesh_attribute_info(
		unsigned int const attribute_id,
		std::vector<TAttr> const data,
		std::vector<unsigned int> const indices)
		: m_id(attribute_id),
		  m_data(data),
		  m_indices(indices)
	{

	}

	template<typename TAttr>
	unsigned int mesh_attribute_info<TAttr>::get_id() const
	{
		return m_id;
	}

	template<typename TAttr>
	std::vector<TAttr> const& mesh_attribute_info<TAttr>::get_data() const
	{
		return m_data;
	}

	template<typename TAttr>
	std::vector<unsigned int> const& mesh_attribute_info<TAttr>::get_indices() const
	{
		return m_indices;
	}
}

#endif
