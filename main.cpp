
#include "config.h"
#include "frame.h"
#include "platform.h"
#include "graphics.h"
#include "frame.h"

// START_EXAMPLE
#include "example_sim.h"
// END_EXAMPLE

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <ratio>


Config default_config;
Platform platform;
Graphics graphics;

Frame frame;

// START_EXAMPLE
EXAMPLE::Sim example_sim;
// END_EXAMPLE

using timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

int main(int argc, char *argv[]) {

	default_config.title = "Black Telephone";
	default_config.viewport_size = { 960, 540 };
	default_config.unit_pixels = 32;
	default_config.update_rate = 60;
	default_config.enable_vsync = false;
	Config::set(default_config);

	const Config& config = Config::get();

	platform.startup();
	graphics.startup();

	std::chrono::duration<u64, std::nano> update_accumulator(0);
	std::chrono::duration<u64, std::milli> update_rate((u32)(config.delta_time * 1000));

	timestamp cur_time = std::chrono::high_resolution_clock::now();

	while (!platform.quitTriggered()) {
		timestamp new_time = std::chrono::high_resolution_clock::now();
		auto frame_time = std::chrono::duration<u64, std::nano>(new_time - cur_time);
		cur_time = new_time;
		update_accumulator += frame_time;
		u32 update_count = 0;
		while (update_accumulator >= update_rate) {
			frame.clear();
			platform.pollEvents(frame);

			// START_EXAMPLE
			example_sim.update(frame);
			// END_EXAMPLE

			update_accumulator -= update_rate;
			update_count++;
			if (update_count > 3) {
				update_accumulator = std::chrono::duration<u64, std::nano>::zero();
			}
		}
		if (update_count > 0 && graphics.visible()) {
			graphics.draw(frame);
		}
	}

	graphics.shutdown();
	platform.shutdown();

	return 0;
}