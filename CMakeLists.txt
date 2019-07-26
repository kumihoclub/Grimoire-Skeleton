cmake_minimum_required(VERSION 3.6)
project (grimoire_skeleton)

set(GRIMOIRE_SKELETON_SRCS
	engine/src/graphics.cpp
	engine/src/frame.cpp
	engine/src/config.cpp
	engine/src/platform.cpp
	)


set(GRIMOIRE_SKELETON_EXAMPLE_SRCS
	example/src/example_sim.cpp
	example/src/example_renderer.cpp
	)

add_executable(grimoire_skeleton main.cpp ${GRIMOIRE_SKELETON_SRCS} ${GRIMOIRE_SKELETON_EXAMPLE_SRCS})

# Grimoire-Skeleton really only reaquires c++11, its just set to 17 here for std::optional in the example_renderer
set_property(TARGET grimoire_skeleton PROPERTY CXX_STANDARD 17)
set_property(TARGET grimoire_skeleton PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(grimoire_skeleton
	opengl32.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL2/lib/SDL2.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL2/lib/SDL2main.lib
	)

include_directories(
	dep
	engine
	utility
	example
	)