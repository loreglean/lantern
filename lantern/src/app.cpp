#include <iostream>
#include <stdexcept>
#include "app.h"

using namespace lantern;

app::app(unsigned int const width, unsigned int const height)
	: m_window{nullptr},
	  m_renderer{nullptr},
	  m_target_texture{nullptr},
	  m_painter{width, height}
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_window = SDL_CreateWindow(
		"lantern",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_target_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (m_target_texture == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}
}

app::~app()
{
	if (m_target_texture != nullptr)
	{
		SDL_DestroyTexture(m_target_texture);
		m_target_texture = nullptr;
	}

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	SDL_Quit();
}

int app::start()
{
	bool running{true};
	SDL_Event event;

#ifdef LANTERN_OUTPUT_FPS
	Uint32 time = SDL_GetTicks();
	unsigned int fps = 0;
#endif

	while(running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}

		update();

		m_painter.clear(0);

		SDL_UpdateTexture(m_target_texture, nullptr, m_painter.get_data(), m_painter.get_pitch());
		SDL_RenderCopy(m_renderer, m_target_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);

#ifdef LANTERN_OUTPUT_FPS
		++fps;

		Uint32 current_time = SDL_GetTicks();
		if (current_time - time > 1000)
		{
			std::cout << "FPS: " << std::to_string(fps) << std::endl;

			time = current_time;
			fps = 0;
		}
#endif
	}

	return 0;
}
