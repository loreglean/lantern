#ifndef LANTERN_POINT_H
#define LANTERN_POINT_H

namespace lantern
{
	class point2D final
	{
	public:
		unsigned int x;
		unsigned int y;

		bool operator==(point2D const& p) const;
	};
}

#endif // LANTERN_POINT_H
