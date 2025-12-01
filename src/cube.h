#ifndef G_CUBE
#define G_CUBE

#include "renderable.h"

typedef struct {
  vec3 pos;
  vec4 color;
  float height, width;
} Cube;

RenderInfo cubeRenderInit();
void cubeRender(Cube *c, RenderInfo i, RenderPayload r);

#endif
