#ifndef LANTERN_RATERIZER_TRAVERSAL_COMMON_H
#define LANTERN_RATERIZER_TRAVERSAL_COMMON_H

#include <vector>
#include "math_common.h"
#include "rasterizer_common.h"

namespace lantern
{
	/** Checks if point lies on edge's positive halfspace, using top-left rule for points on the edge
	* @param edge_equation_value Value of edge equation in the point
	* @param edge_equation_a X-coefficient of edge equation
	* @param edge_equation_b Y-coefficient of edge equation
	* @returns True if point is either inside positive halfplane or it is on a left or a top edge
	*/
	bool is_point_on_positive_halfspace_top_left(
		float const edge_equation_value, float const edge_equation_a, float const edge_equation_b);

	/** Sets binds points values basing on barycentric coordinates
	* @param binds List of binds
	* @param index0 First triangle vertex index in a mesh
	* @param index1 Second triangle vertex index in a mesh
	* @param index2 Third triangle vertex index in a mesh
	* @param b0 First barycentric coordinate
	* @param b1 Second barycentric coordinate
	* @param b2 Third barycentric coordinate
	* @param z0_view_space_reciprocal 1/z-view for first vertex
	* @param z1_view_space_reciprocal 1/z-view for second vertex
	* @param z1_view_space_reciprocal 1/z-view for third vertex
	*/
	template<typename TAttr>
	void set_bind_points_values_from_barycentric(
		std::vector<binded_mesh_attribute_info<TAttr>>& binds,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		float const b0, float const b1, float const b2,
		float const z0_view_space_reciprocal, float const z1_view_space_reciprocal, float const z2_view_space_reciprocal)
	{
		size_t binds_count{binds.size()};
		for (size_t i{0}; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr>& binded_attr = binds[i];
			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();

			TAttr const& value0 = binded_attr_data[binded_attr_indices[index0]];
			TAttr const& value1 = binded_attr_data[binded_attr_indices[index1]];
			TAttr const& value2 = binded_attr_data[binded_attr_indices[index2]];

			if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::linear)
			{
				(*binded_attr.bind_point) = value0 * b0 + value1 * b1 + value2 * b2;
			}
			else if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::perspective_correct)
			{
				TAttr const value0_div_zview = value0 * z0_view_space_reciprocal;
				TAttr const value1_div_zview = value1 * z1_view_space_reciprocal;
				TAttr const value2_div_zview = value2 * z2_view_space_reciprocal;

				float const zview_reciprocal_interpolated = z0_view_space_reciprocal * b0 + z1_view_space_reciprocal * b1 + z2_view_space_reciprocal * b2;
				TAttr value_div_zview_interpolated = value0_div_zview * b0 + value1_div_zview * b1 + value2_div_zview * b2;

				(*binded_attr.bind_point) = value_div_zview_interpolated * (1.0f / zview_reciprocal_interpolated);
			}
		}
	}
}

#endif