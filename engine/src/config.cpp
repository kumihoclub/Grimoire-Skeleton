
#include "config.h"

namespace {

	Config stored_config;
	b32 config_set = false;

}

void Config::set(Config& config) {
	if (!config_set) {
		stored_config = config;
		// Stored the delta time from the update rate
		stored_config.delta_time = (1.0 / (f32)config.update_rate);
		config_set = true;
	}
}

const Config& Config::get() {
	return stored_config;
}
