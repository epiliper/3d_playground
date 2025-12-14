#ifndef G_CUBE
#define G_CUBE

#include "renderable.h"

typedef struct {
  vec3 pos;
  vec4 color;
  float height, width;
} Cube;

// create buffers, copy vertex data, init shaders
RenderInfo cubeRenderInit();

void cubeRender(Cube *c, RenderInfo i, RenderPayload r, RenderMods *mods);

// update a supplied matrix to the model matrix of the cube.
void cubeUpdateModel(Cube *c, mat4 *dest);

void cubeMove(Cube *c, vec3 pos);

void cubePos(Cube *c, vec3 pos);

#endif
