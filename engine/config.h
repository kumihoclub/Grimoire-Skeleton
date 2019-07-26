#pragma once

/*
While it is perfectly fine to edit all of these
values here, I would recommened editing them in
the main like I do before instantiating all the
subsystems.
*/

#include "common.h"

#include "glm/glm.hpp"

#include <string>

struct Config {
	std::string title = "Grimoire-Skeleton";
	glm::uvec2 viewport_size = { 960, 540 };
	u32 unit_pixels = 32;
	u32 update_rate = 240;
	u32 render_rate = 60;
	f32 delta_time = 0.0f;
	b32 enable_vsync = true;
	static void set(Config& config);
	static const Config& get();
};