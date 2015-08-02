#ifndef LANTERN_AABB_H
#define LANTERN_AABB_H

namespace lantern
{
	/** Class that represents axis-aligned bounding box */
	template<typename TPoint>
	class aabb final
	{
	public:
		/** Starting point */
		TPoint from;

		/** End point */
		TPoint to;
	};
}

#endif // LANTERN_AABB_H