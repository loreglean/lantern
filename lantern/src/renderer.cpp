#include "renderer.h"

using namespace lantern;

renderer::renderer()
{

}

geometry_stage& renderer::get_geometry_stage()
{
	return m_geometry_stage;
}

rasterizing_stage& renderer::get_rasterizing_stage()
{
	return m_rasterizing_stage;
}

merging_stage& renderer::get_merging_stage()
{
	return m_merging_stage;
}