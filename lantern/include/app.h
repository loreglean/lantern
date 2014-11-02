#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#include <SDL2/SDL.h>
#include "bitmap_painter.h"

namespace lantern
{
	class app
	{
	public:
		app(unsigned int const width, unsigned int const height);
		virtual ~app();

		// Runs main loop
		int start();

	protected:
		bitmap_painter& get_painter();

		virtual void frame(float delta_since_last_frame) = 0;
		virtual void on_key_down(SDL_Keysym key);

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_target_texture;
		bitmap_painter m_painter;
	};
}

#endif
