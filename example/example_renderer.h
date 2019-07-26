#pragma once

/*
This is purely an EXAMPLE renderer object that renders a triangle whos rotation
is affected by the EXAMPLE.rotation value in the Frame object. It is not required
for Grimiore-Skeleton to function.

Both the EXAMPLE sim and the EXAMPLE renderer combined with EXAMPLE.rotation
in the Frame object exist just to show the pipeline of data being passed to
the Graphics object. In this scenario it is nothing more than a rotation fed
to a model matrix uniform in the EXAMPLE renderer object, but in your own
setup you might pass custom draw commands or post processing state to be
consumed by your own custom renderer type and drawn.
*/

#include "common.h"
#include "config.h"
#include "frame.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <optional>
#include <vector>

namespace EXAMPLE {

	class Renderer {
	public:
		void startup();
		void shutdown();
		void draw(Frame& frame, const glm::vec2& viewport);
	private:
		std::optional<u32> m_vbo;
		std::optional<u32> m_vao;
		std::optional<u32> m_shader;
		std::optional<u32> m_model_uniform;
	};

}