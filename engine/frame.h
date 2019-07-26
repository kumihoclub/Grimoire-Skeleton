#pragma once

#include "common.h"

#include "SDL2/SDL.h"

#include <vector>

class Frame {
public:
	// START_EXAMPLE
	struct {
		f32 rotation = 0.0f;
	} EXAMPLE;
	// END_EXAMPLE
	struct InputEvent {
		SDL_Scancode scancode;
		b32 state = false;
	};
	std::vector<InputEvent> input_events;
	// Just set to true when needing to toggle, the graphics object auto sets it back to false after updating.
	b32 toggle_fullscreen = false;
	void clear();
};