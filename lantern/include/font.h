#ifndef LANTERN_FONT_H
#define LANTERN_FONT_H

#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "texture.h"
#include "vector2.h"

namespace lantern
{
	/** Represents a symbol from a font. It holds symbol's texture and all the metrics required for rendering */
	class symbol final
	{
	public:
		/** Constructor that takes required information from a FreeType glyph
		* @param ft_glyp FreeType glyph object
		*/
		symbol(FT_GlyphSlot ft_glyph);

		/** Gets symbol texture (in grayscale)
		* @returns Texture
		*/
		texture const& get_texture() const;

		/** Gets symbol's size in pixels
		* @returns Symbols's size
		*/
		vector2ui get_size() const;

		/** Gets bearing metrics in pixels
		* @returns Bearing metrics
		*/
		vector2ui get_bearing() const;

		/** Gets symbol's advance in pixels
		* @returns Advance length
		*/
		unsigned int get_advance() const;

	private:
		/** Symbol's texture */
		texture m_texture;

		/** Size in pixels */
		vector2ui const m_size;

		/** Bearing metrics */
		vector2ui const m_bearing;

		/** Advance in pixels */
		unsigned int const m_advance;
	};

	/** This class is responsible for loading and keeping symbols. Symbols are only loaded when required and cached afterwards. Supports only ASCII for now */
	class font final
	{
	public:
		/** Initializes instance with specified truetype font and required size
		* @param font_file_path Path to the font to load to
		* @param size Font size
		*/
		font(std::string font_file_path, int size);

		/** Cleans up all the resources */
		~font();

		/** Gets symbol object for a specified character
		* @param c Character
		* @returns Symbol object for the character
		*/
		symbol const* get_symbol(char c);

	private:
		/** FreeType font object */
		FT_Face m_font;

		/** Dictionary of already loaded symbols: character -> symbol object */
		std::map<char, symbol const*> m_symbols_map;
	};
}

#endif // LANTERN_FONT_H