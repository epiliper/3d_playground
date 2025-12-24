#include "entity.h"
#include "cube.h"
#include "defs.h"

void entityLoadFromData(void *data, uint8_t type, Body loc, Entity *dest) {

  Renderable render;
  render.init = RENDER_UNINIT;

  switch (type) {
  case ENT_CUBE:
    render.rfunc = (RenderFunc)cubeRender;
    render.rinitfunc = (RenderInitFunc)cubeRenderInit;
    break;
  case ENT_GRID:
    render.rfunc = (RenderFunc)gridRender;
    render.rinitfunc = gridRenderInit;
    break;
  case ENT_LINE:
    render.rfunc = (RenderFunc)lineRender;
    render.rinitfunc = lineRenderInit;
    break;
  }

  *dest = (Entity){
      .type = type,
      .render = render,
      .loc = loc,
      .data = data,
  };
}
