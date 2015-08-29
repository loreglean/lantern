#ifndef LANTERN_UI_LABEL_H
#define LANTERN_UI_LABEL_H

#include <string>
#include <vector>
#include "ui_element_base.h"
#include "font.h"
#include "mesh.h"
#include "ui_label_shader.h"

namespace lantern
{
	/** UI Label
	* @ingroup UI
	*/
	class ui_label final : public ui_element_base
	{
	public:
		/** Creates label object with specified font to use for rendering
		* @param font Font to use for rendering
		* @param target_texture Texture that will be used for rendering, required for calculating some metrics before rendering
		*/
		ui_label(font& font, texture const& target_texture);

		/** Changes label's text
		* @param text New text
		*/
		void set_text(std::string const& text);

		/** Changes label's color
		* @param color New color
		*/
		void set_color(color const& color);

		/** Draws the label and all its children
		* @param pipeline Pipeline to use
		* @param target_texture Texture to draw to
		*/
		void draw(renderer& pipeline, texture& target_texture) override;

	protected:
		/** Invokes whenever position is changed */
		void on_position_changed() override;

	private:
		/** Recreates internal mesh for symbols */
		void update_mesh();

		/** How much do we move in ndc space for one pixel in screen space */
		vector2f const m_ndc_per_pixel;

		/** Shader to use */
		ui_label_shader m_shader;

		/** Font object to use */
		font& m_font;

		/** Label's text */
		std::string m_text;

		/** Label's color */
		color m_color;

		/** Current symbols to use */
		std::vector<symbol const*> m_symbols;

		/** Current meshes to use */
		std::vector<mesh> m_meshes;
	};
}

#endif // LANTERN_UI_LABEL_H