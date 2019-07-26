#pragma once

#include "common.h"
#include "frame.h"

class Platform {
public:
	void startup();
	void shutdown();
	void pollEvents(Frame& frame);
	b32 quitTriggered();
private:
	b32 m_quit_triggered = false;
};
