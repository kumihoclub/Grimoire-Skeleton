
#include "graphics.h"
#include "config.h"

#include "glad/glad.h"
#include "glad/src/glad.c"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

void Graphics::startup() {

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

	const Config& config = Config::get();
	m_window = SDL_CreateWindow(config.title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		config.viewport_size.x, config.viewport_size.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	m_current_veiwport = config.viewport_size;

	m_thread = std::thread([&]() -> b32 {

		SDL_GLContext context = SDL_GL_CreateContext(m_window);
		SDL_GL_MakeCurrent(m_window, context);

		if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			SDL_GL_DeleteContext(context);
			return 0;
		}

		const Config& config = Config::get();
		if (config.enable_vsync) {
			SDL_GL_SetSwapInterval(1);
		}

		glViewport(0, 0, m_current_veiwport.x, m_current_veiwport.y);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		// START_EXAMPLE
		m_exmaple_renderer.startup();
		// END_EXAMPLE

		m_running = true;

		while (m_running) {
			std::unique_lock<std::mutex> l(m_frame_lock);
			m_frame_cond.wait(l, [&]() {
				return m_frame_in_flight.load();
			});
			if (!m_running) {
				break;
			}
			if (m_frame.toggle_fullscreen && !m_fullscreen) {
				SDL_DisplayMode current;
				SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(m_window), &current);
				SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				m_current_veiwport = { current.w , current.h };
				glViewport(0, 0, m_current_veiwport.x, m_current_veiwport.y);
				m_fullscreen = true;
				m_frame.toggle_fullscreen = false;
			}
			else if (m_frame.toggle_fullscreen && m_fullscreen) {
				SDL_ShowCursor(SDL_ENABLE);
				SDL_SetWindowFullscreen(m_window, 0);
				m_current_veiwport = { config.viewport_size.x , config.viewport_size.y };
				glViewport(0, 0, m_current_veiwport.x, m_current_veiwport.y);
				m_fullscreen = false;
				m_frame.toggle_fullscreen = false;
			}
			glClear(GL_COLOR_BUFFER_BIT);

			// START_EXAMPLE
			m_exmaple_renderer.draw(m_frame, m_current_veiwport);
			// END_EXAMPLE

			m_frame_in_flight.store(false);
			SDL_GL_SwapWindow(m_window);
		}

		m_exmaple_renderer.shutdown();

		SDL_GL_DeleteContext(context);
		return 0;

	});

}

b32 Graphics::visible() {
	return !(SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED);
}

void Graphics::draw(Frame& frame) {
	b32 expected = false;
	if (m_frame_in_flight.compare_exchange_weak(expected, true)) {
		std::swap(frame, m_frame);
		m_frame_cond.notify_all();
	}
}

void Graphics::shutdown() {
	m_running = false;
	m_frame_in_flight = true;
	m_frame_cond.notify_all();
	m_thread.join();
}