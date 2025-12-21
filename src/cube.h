#ifndef G_CUBE
#define G_CUBE

#include "renderable.h"
#include "entity.h"

typedef struct {
  vec4 color;
} Cube;

typedef struct {
  vec4 line_color;
} Grid;

// create buffers, copy vertex data, init shaders
RenderInfo cubeRenderInit();

void cubeRender(Cube *c, Body *body, RenderInfo rinfo, RenderPayload r,
                RenderMods *mods);

// update a supplied matrix to the model matrix of the cube.
void cubeUpdateModel(Cube *c, mat4 *dest);

void cubeMove(Cube *c, vec3 pos);

void cubePos(Cube *c, vec3 pos);

// SQUARE
RenderInfo gridRenderInit();
void gridRender(Grid *g, Body *body, RenderInfo rinfo, RenderPayload r,
                RenderMods *mods);

#endif
