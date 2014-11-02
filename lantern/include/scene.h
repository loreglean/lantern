#ifndef LANTERN_SCENE_H
#define LANTERN_SCENE_H

#include <vector>
#include "scene_object.h"
#include "camera.h"

namespace lantern
{
	class scene final
	{
	public:
		std::vector<scene_object>& get_objects();
		std::vector<scene_object> const& get_objects() const;

	private:
		std::vector<scene_object> m_objects;
	};
}

#endif
