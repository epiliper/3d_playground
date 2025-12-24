#include "entity.h"
#include "cube.h"
#include "defs.h"

void entityLoadFromData(void *data, uint8_t type, Body loc, Entity *dest) {

  Renderable render;

  switch (type) {
  case ENT_CUBE:
    render.rfunc = (RenderFunc)cubeRender;
    render.rinfo = cubeRenderInit();
    break;
  case ENT_GRID:
    render.rfunc = (RenderFunc)gridRender;
    render.rinfo = gridRenderInit();
    break;

  case ENT_LINE:
    render.rfunc = (RenderFunc)lineRender;
    render.rinfo = lineRenderInit();
    break;
  }

  *dest = (Entity){
      .type = type,
      .render = render,
      .loc = loc,
      .data = data,
  };
}
