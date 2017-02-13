#ifndef MAIN_H
#define MAIN_H
#include "scene.h"
#include "lua/lua.hpp"
#include <iostream>
#include <vector>
#include <Block.h>

// This controls if the rendering displays progressively. When timing the results, turn this off (otherwise updating the window repeatedly will affect the timing)
// progressiveCount is how many lines are rendered before updating the window.
extern bool progressiveDisplay;
extern int  progressiveCount;

struct Block;
typedef struct block * Block;
// The resolution of the window and the output of the ray tracer. This can be overridden by the Lua startup script.
extern int windowWidth;
extern int windowHeight;

std::vector<Block> MapOut(int width, int height);
// The scene object.
extern Scene g_scene;

// Lua state object used to run the startup script.
extern lua_State *g_state;

#endif
