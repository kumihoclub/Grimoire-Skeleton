#pragma once

#include "common.h"
#include "frame.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

// START_EXAMPLE
#include "example_renderer.h"
// END_EXAMPLE

class Graphics {
public:
	void startup();
	void shutdown();
	b32 visible();
	void draw(Frame& frame);
private:
	SDL_Window* m_window = nullptr;
	b32 m_fullscreen = false;
	glm::vec2 m_current_veiwport = { 0.0f, 0.0f };
	Frame m_frame;

	std::thread m_thread;
	std::atomic<b32> m_running = false;
	std::atomic<b32> m_frame_in_flight = false;
	std::condition_variable m_frame_cond;
	std::mutex m_frame_lock;

	// START_EXAMPLE
	EXAMPLE::Renderer m_exmaple_renderer;
	// END_EXAMPLE

};