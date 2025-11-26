#ifndef G_DUMMY
#define G_DUMMY

#include "renderable.h"

typedef struct {
  RenderInfo rinfo;
  vec3 pos;
  vec4 color;
  float height, width;
} Cube;

extern Cube testCube;

void testScenePrepare();
void testSceneRender();

#endif
