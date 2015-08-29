#include "rasterizing_stage.h"

using namespace lantern;

rasterizing_stage::rasterizing_stage()
	: m_rasterization_algorithm{rasterization_algorithm_option::homogeneous}
{

}

void rasterizing_stage::set_rasterization_algorithm(rasterization_algorithm_option algorithm_option)
{
	m_rasterization_algorithm = algorithm_option;
}

rasterization_algorithm_option rasterizing_stage::get_rasterization_algorithm() const
{
	return m_rasterization_algorithm;
}