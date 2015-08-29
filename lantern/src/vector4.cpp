#include "vector4.h"

using namespace lantern;

vector4f::vector4f()
	: x{0.0f}, y{0.0f}, z{0.0f}, w{0.0f}
{
}

vector4f::vector4f(float const x, float const y, float const z, float const w)
	: x{x}, y{y}, z{z}, w{w}
{

}