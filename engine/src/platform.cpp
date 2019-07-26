
#include "platform.h"

#include <iostream>

void Platform::startup() {
	u32 init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
	/*
	There is an issue with in SDL 2.0.9 on Windows where hid_enumerate() cause everything to hang up every 3 seconds or so;
	The workaround is to call SDL_Init with SDL_INIT_JOYSTICK flagged even if you are not going to support joysticks;

	Link: https://bugzilla.libsdl.org/show_bug.cgi?id=4391

	Going to do an SDL version check to check for 2.0.9 and apply the workaround if needed;
	*/
#ifdef _WIN32
	SDL_version sdl_ver;
	SDL_GetVersion(&sdl_ver);
	if (sdl_ver.patch == 9 && !(init_flags & SDL_INIT_JOYSTICK)) {
		init_flags = (init_flags | SDL_INIT_JOYSTICK);
	}
#endif
	SDL_Init(init_flags);
}

void Platform::shutdown() {
	SDL_Quit();
}

void Platform::pollEvents(Frame& frame) {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if ((e.type == SDL_KEYUP || e.type == SDL_KEYDOWN) && e.key.repeat == 0) { // Dont store repeat events
			Frame::InputEvent input_event;
			input_event.scancode = e.key.keysym.scancode;
			if (e.type == SDL_KEYDOWN) {
				input_event.state = true;
			}
			else {
				input_event.state = false;
			}
			frame.input_events.push_back(input_event);
		}
		else if (e.type == SDL_QUIT) {
			m_quit_triggered = true;
		}
	}
}

b32 Platform::quitTriggered() {
	return m_quit_triggered;
}