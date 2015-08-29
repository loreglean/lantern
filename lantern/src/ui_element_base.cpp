#include "ui_element_base.h"

using namespace lantern;

ui_element_base::~ui_element_base()
{

}

void ui_element_base::set_position(vector2f position)
{
	m_position = position;

	on_position_changed();
}

void ui_element_base::draw(renderer& pipeline, texture& target_texture)
{
	for (ui_element_base* element : m_children)
	{
		element->draw(pipeline, target_texture);
	}
}

std::vector<ui_element_base*> ui_element_base::get_children_storage()
{
	return m_children;
}

void ui_element_base::on_position_changed()
{

}