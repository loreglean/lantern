#ifndef LANTERN_POINT_H
#define LANTERN_POINT_H

namespace lantern
{
	class point2d final
	{
	public:
		unsigned int x;
		unsigned int y;

		bool operator==(point2d const& p) const;
	};
}

#endif // LANTERN_POINT_H
