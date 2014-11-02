#include "scene_object.h"

using namespace lantern;

scene_object::scene_object(std::shared_ptr<mesh const> mesh)
	: m_mesh{mesh}, m_world_position{vector3::ZERO}, m_world_rotation{vector3::ZERO}
{

}

std::shared_ptr<mesh const> scene_object::get_mesh() const
{
	return m_mesh;
}

vector3 scene_object::get_world_position() const
{
	return m_world_position;
}

vector3 scene_object::get_world_rotation() const
{
	return m_world_rotation;
}

vector3& scene_object::get_world_rotation()
{
	return m_world_rotation;
}

void scene_object::set_world_position(vector3 const& position)
{
	m_world_position = position;
}

void scene_object::set_world_rotation(vector3 const& rotation)
{
	m_world_rotation = rotation;
}
