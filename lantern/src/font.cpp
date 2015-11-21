#include <stdexcept>
#include "font.h"
#include "app.h"

using namespace lantern;

// Symbol class implementation
//

symbol::symbol(FT_GlyphSlot ft_glyph)
	: m_texture{ft_glyph->bitmap.width, ft_glyph->bitmap.rows},
	  m_size{ft_glyph->bitmap.width, ft_glyph->bitmap.rows},
	  m_bearing{ft_glyph->bitmap_left, ft_glyph->bitmap_top},
	  m_advance(ft_glyph->advance.x >> 6) // FreeType keeps advance in 1/64 pixel
{
	// Create texture object from the bitmap FreeType created
	//
	for (size_t y{0}; y < m_size.y; ++y)
	{
		for (size_t x{0}; x < m_size.x; ++x)
		{
			// Bitmap is 8-bit grayscale
			//

			// Take pixel channel
			unsigned char const grayscale{ft_glyph->bitmap.buffer[y * m_size.x + x]};

			// Normalize it to pass to our color object
			float const grayscale_normalized{grayscale / 255.0f};

			// Set pixel color in the texture
			m_texture.set_pixel_color(
				vector2ui{x, y},
				color{grayscale_normalized, grayscale_normalized, grayscale_normalized, grayscale_normalized});
		}
	}
}

texture const& symbol::get_texture() const
{
	return m_texture;
}
vector2ui symbol::get_size() const
{
	return m_size;
}

vector2ui symbol::get_bearing() const
{
	return m_bearing;
}

unsigned int symbol::get_advance() const
{
	return m_advance;
}

// Font class implementation
//

font::font(std::string font_file_path, int size)
{
	// Load FreeType font object
	//
	if (FT_New_Face(app::get_instance()->get_freetype_library(), font_file_path.c_str(), 0, &m_font))
	{
		throw std::runtime_error("Couldn't initialize font");
	}

	// Set size. Width will be calculated automatically base on height we provide
	FT_Set_Pixel_Sizes(m_font, 0, size);
}

font::~font()
{
	// Clean up
	//

	for (auto const& kvp : m_symbols_map)
	{
		delete kvp.second;
	}

	FT_Done_Face(m_font);
}

symbol const* font::get_symbol(char c)
{
	// Check if we already got that symbol's metrics
	std::map<char, symbol const*>::const_iterator find_result = m_symbols_map.find(c);

	if (find_result == m_symbols_map.end())
	{
		// If we didn't - get it
		//

		// Load metrics and render bitmap
		//
		if (FT_Load_Char(m_font, c, FT_LOAD_RENDER))
		{
			throw std::runtime_error("Couldn't load glyph");
		}

		// Cache it
		m_symbols_map.insert(std::make_pair(c, new symbol{m_font->glyph}));

		// Return the result
		return m_symbols_map.find(c)->second;
	}
	else
	{
		// Just return cached symbol
		return find_result->second;
	}
}