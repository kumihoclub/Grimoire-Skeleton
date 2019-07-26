#pragma once

/*
This is purely an EXAMPLE sim object that feeds a rotation value into
EXAMPLE.rotation held in frame to be fed to the EXAMPLE renderer as the
frame is passed down the pipeline. It is not required for Grimiore-Skeleton
o function.

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

namespace EXAMPLE {

	class Sim {
	public:
		void update(Frame& frame);
	private:
		f32 m_rotation = 0.0f;
		b32 m_a_held = false;
		b32 m_d_held = false;
	};

}