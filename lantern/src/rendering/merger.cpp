#include "merger.h"

using namespace lantern;

merger::merger()
	: m_alpha_blending_enabled{false}
{

}

void merger::set_alpha_blending_enabled(bool const enabled)
{
	m_alpha_blending_enabled = enabled;
}

void merger::merge(texture& target_texture, vector2ui const& pixel_coordinates, color const& pixel_color) const
{
	if (!m_alpha_blending_enabled)
	{
		target_texture.set_pixel_color(pixel_coordinates, pixel_color);
	}
	else
	{
		color const current_color = target_texture.get_pixel_color(pixel_coordinates);
		color const result_color = pixel_color * pixel_color.a + current_color * (1.0f - pixel_color.a);
		target_texture.set_pixel_color(pixel_coordinates, result_color);
	}
}
