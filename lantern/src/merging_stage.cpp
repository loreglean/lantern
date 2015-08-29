#include "merging_stage.h"

using namespace lantern;

merging_stage::merging_stage()
	: m_alpha_blending_enabled{false}
{

}

bool merging_stage::get_alpha_blending_enabled() const
{
	return m_alpha_blending_enabled;
}

void merging_stage::set_alpha_blending_enabled(bool const enabled)
{
	m_alpha_blending_enabled = enabled;
}