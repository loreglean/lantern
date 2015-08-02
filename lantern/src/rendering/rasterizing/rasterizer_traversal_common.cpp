#include "rasterizer_traversal_common.h"

using namespace lantern;

bool lantern::is_point_on_positive_halfspace_top_left(
	float const edge_equation_value, float const edge_equation_a, float const edge_equation_b)
{
	// If we are on the edge, use top-left filling rule
	//
	if (std::abs(edge_equation_value) < FLOAT_EPSILON)
	{
		// edge_equation_value == 0.0f, thus point is on the edge,
		// and we use top-left rule to decide if point is inside a triangle
		//
		if (std::abs(edge_equation_a) < FLOAT_EPSILON)
		{
			// edge.a == 0.0f, thus it's a horizontal edge and is either a top or a bottom one
			//

			// If normal y coordinate is pointing up - we are on the top edge
			// Otherwise we are on the bottom edge
			return edge_equation_b > 0.0f;
		}
		else
		{
			// If normal x coordinate is pointing right - we are on the left edge
			// Otherwise we are on the right edge
			return edge_equation_a > 0.0f;
		}
	}
	else
	{
		// Check if we're on a positive halfplane
		return edge_equation_value > 0.0f;
	}
}