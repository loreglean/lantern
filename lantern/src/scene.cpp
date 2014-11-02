#include "scene.h"

using namespace lantern;

std::vector<scene_object>& scene::get_objects()
{
	return m_objects;
}

std::vector<scene_object> const &scene::get_objects() const
{
	return m_objects;
}
