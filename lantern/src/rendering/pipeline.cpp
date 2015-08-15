#include "pipeline.h"

using namespace lantern;

pipeline::pipeline()
{
	set_rasterization_algorithm(rasterization_algorithm_option::homogeneous);
	set_face_culling(face_culling_option::counter_clockwise);
	set_fill_mode(fill_mode_option::solid);
}

merger& pipeline::get_merger()
{
	return m_merger;
}

void pipeline::set_rasterization_algorithm(rasterization_algorithm_option value)
{
	m_rasterization_algorithm = value;
}

void pipeline::set_face_culling(face_culling_option value)
{
	m_face_culling = value;
}

void pipeline::set_fill_mode(fill_mode_option value)
{
	m_fill_mode = value;
}
