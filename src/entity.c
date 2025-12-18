#include "entity.h"
#include "cube.h"

void entityLoadFromData(void *data, uint8_t type, Body loc, Entity *dest) {

  Renderable render;

  switch (type) {
  case ENT_CUBE:
    render.rfunc = (RenderFunc)cubeRender;
    render.rinfo = cubeRenderInit();
    break;
  }

  *dest = (Entity){
      .type = ENT_CUBE,
      .render = render,
      .loc = loc,
      .data = data,
  };
}
