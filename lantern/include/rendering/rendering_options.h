#ifndef LANTERN_RENDERING_OPTIONS
#define LANTERN_RENDERING_OPTIONS

namespace lantern
{
	/** Face culling options */
	enum class face_culling_option
	{
		/** Visible wind order is clockwise */
		clockwise,

		/** Visible wind order is counter clockwise */
		counter_clockwise
	};

	/** Rasterization algorithms option */
	enum class rasterization_algorithm_option
	{
		/** Rasterization using traversal algorithm with axis-aligned bounding box */
		traversal_aabb,

		/** Rasterization using traversal algorithm with backtracking */
		traversal_backtracking,

		/** Rasterization using traversal algorithm changing direction on every line */
		traversal_zigzag,

		/** Rasterization using inversed slope algorithm */
		inversed_slope,

		/** Rasterization using homogeneous 2d coordinates algorithm.
		* This option uses perspective corrected interpolation for all attributes despite of what is specified */
		homogeneous
	};

	enum class fill_mode_option
	{
		/** Solid triangle rasterization */
		solid,

		/** Wireframe triangle rasterization (edges only) */
		wireframe
	};
}

#endif