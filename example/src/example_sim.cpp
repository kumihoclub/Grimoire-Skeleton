
#include "example_sim.h"

#include <iostream>

void EXAMPLE::Sim::update(Frame& frame) {

	const Config& config = Config::get();

	for (const auto& e : frame.input_events) {
		if (e.scancode == SDL_SCANCODE_A) {
			if (e.state == true) m_a_held = true;
			else if (e.state == false) m_a_held = false;
		}
		else if (e.scancode == SDL_SCANCODE_D) {
			if (e.state == true) m_d_held = true;
			else if (e.state == false) m_d_held = false;
		}
	}
	if (m_a_held) {
		m_rotation += (120.0f * config.delta_time); // 120 degress every second
	}
	if (m_d_held) {
		m_rotation -= (120.0f * config.delta_time); // 120 degress every second
	}

	frame.EXAMPLE.rotation = m_rotation;

}