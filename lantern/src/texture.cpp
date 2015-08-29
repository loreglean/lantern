#include <stdexcept>
#include <SDL_image.h>
#include "texture.h"

using namespace lantern;

texture::texture(unsigned int const width, unsigned int const height)
	: m_width{width},
	m_height{height},
	m_data_total_size{width * height * 4},
	m_data{new unsigned char[m_data_total_size]},
	m_pitch{width * 4}
{
}

texture::texture(texture const& another)
	: m_width{another.m_width},
	m_height{another.m_height},
	m_data_total_size{another.m_data_total_size},
	m_data{nullptr},
	m_pitch{another.m_pitch}
{
	m_data = new unsigned char[m_data_total_size];
	memcpy(m_data, another.m_data, m_data_total_size);
}

texture::texture(texture&& another)
	: m_width(another.m_width),
	m_height(another.m_height),
	m_data_total_size(another.m_data_total_size),
	m_data{another.m_data},
	m_pitch(another.m_pitch)
{
	another.m_data = nullptr;
}

texture::~texture()
{
	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
}

unsigned int texture::get_width() const
{
	return m_width;
}

unsigned int texture::get_height() const
{
	return m_height;
}

unsigned int texture::get_pitch() const
{
	return m_pitch;
}

unsigned char const* texture::get_data() const
{
	return m_data;
}

texture texture::load_from_file(std::string file)
{
	SDL_Surface* surface = IMG_Load(file.c_str());

	if (surface->format->format == SDL_PIXELFORMAT_ARGB8888)
	{
		texture result(surface->w, surface->h);
		memcpy(result.m_data, surface->pixels, result.m_data_total_size);
		SDL_FreeSurface(surface);
		return result;
	}
	else if (surface->format->format == SDL_PIXELFORMAT_ABGR8888)
	{
		Uint8* pixels = (Uint8*)surface->pixels;

		texture result(surface->w, surface->h);
		for (size_t i = 0; i < result.m_data_total_size; i += 4)
		{
			size_t first_byte_index = i;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			result.m_data[first_byte_index + 0] = pixels[first_byte_index + 0];
			result.m_data[first_byte_index + 1] = pixels[first_byte_index + 3];
			result.m_data[first_byte_index + 2] = pixels[first_byte_index + 2];
			result.m_data[first_byte_index + 3] = pixels[first_byte_index + 1];
#else
			result.m_data[first_byte_index + 0] = pixels[first_byte_index + 2];
			result.m_data[first_byte_index + 1] = pixels[first_byte_index + 1];
			result.m_data[first_byte_index + 2] = pixels[first_byte_index + 0];
			result.m_data[first_byte_index + 3] = pixels[first_byte_index + 3];
#endif
		}
		SDL_FreeSurface(surface);
		return result;
	}
	else
	{
		SDL_FreeSurface(surface);
		throw std::runtime_error{"Unsupported format"};
	}
}