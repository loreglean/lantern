#include "matrix3x3.h"

using namespace lantern;

matrix3x3 const matrix3x3::IDENTITY = matrix3x3{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f};
