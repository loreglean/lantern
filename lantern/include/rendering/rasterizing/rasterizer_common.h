#ifndef LANTERN_RASTERIZER_COMMON_H
#define LANTERN_RASTERIZER_COMMON_H

#include <vector>
#include "color.h"
#include "vector2.h"
#include "vector3.h"
#include "mesh_attribute_info.h"

namespace lantern
{
	/** Class that holds information about binded attribute: its info and bind point */
	template<typename TAttr>
	class binded_mesh_attribute_info final
	{
	public:
		/** Attribute data */
		mesh_attribute_info<TAttr> const& info;

		/** Address of variable to put interpolated value into */
		TAttr* bind_point;
	};

	/** Container for all the binds */
	class binded_mesh_attributes final
	{
	public:
		std::vector<binded_mesh_attribute_info<color>> color_attributes;
		std::vector<binded_mesh_attribute_info<float>> float_attributes;
		std::vector<binded_mesh_attribute_info<vector2f>> vector2f_attributes;
		std::vector<binded_mesh_attribute_info<vector3f>> vector3f_attributes;
	};
}

#endif // LANTERN_RASTERIZER_COMMON_H