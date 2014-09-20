#ifndef LANTERN_APP_H
#define LANTERN_APP_H

#include <SDL2/SDL.h>
#include "bitmap_painter.h"

namespace lantern
{
	class app
	{
	public:
		app(unsigned int width, unsigned int height);
		virtual ~app();

		int start();

	protected:
		virtual void update() = 0;

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_target_texture;
		bitmap_painter m_painter;
	};
}

#endif
