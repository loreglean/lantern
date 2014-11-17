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

	// Time when last frame was executed
	Uint32 last_frame_time{0};

	Uint32 time_accumulator{0};
	unsigned int fps{0};

	while(running)
	{
		// Process events
		//
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				on_key_down(event.key.keysym);
			}
		}

		// Clear texture
		m_painter.clear(0);

		// Calculate time since last frame
		//
		Uint32 current_time{SDL_GetTicks()};
		Uint32 delta_since_last_frame{current_time - last_frame_time};

		// Save last frame time
		last_frame_time = SDL_GetTicks();

		// Execute frame
		frame(delta_since_last_frame / 1000.0f);

		// Sum up passed time
		time_accumulator += delta_since_last_frame;

		// Present texture
		//
		SDL_UpdateTexture(m_target_texture, nullptr, m_painter.get_data(), m_painter.get_pitch());
		SDL_RenderCopy(m_renderer, m_target_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);

		++fps;

		if (time_accumulator >= 1000)
		{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
			std::cout << "FPS: " << fps << std::endl;
#endif
			time_accumulator = 0;
			fps = 0;
		}
	}

	return 0;
}

bitmap_painter& app::get_painter()
{
	return m_painter;
}

void app::on_key_down(SDL_Keysym key)
{

}
