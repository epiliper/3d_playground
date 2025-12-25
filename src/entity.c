#include "entity.h"
#include "cube.h"
#include "defs.h"
#include "string.h" //memcpy

void entityLoadFromData(void *data, uint8_t type, Body loc, Entity *dest) {

  Renderable render;
  render.init = RENDER_UNINIT;

  switch (type) {
  case ENT_CUBE:
    render.rfunc = (RenderFunc)cubeRender;
    render.rinitfunc = (RenderInitFunc)cubeRenderInit;
    dest->data = malloc(sizeof(Cube));
    memcpy(dest->data, data, sizeof(Cube));
    break;
  case ENT_GRID:
    render.rfunc = (RenderFunc)gridRender;
    render.rinitfunc = gridRenderInit;
    dest->data = malloc(sizeof(Grid));
    memcpy(dest->data, data, sizeof(Grid));
    break;
  case ENT_LINE:
    render.rfunc = (RenderFunc)lineRender;
    render.rinitfunc = lineRenderInit;
    dest->data = malloc(sizeof(Line2D));
    memcpy(dest->data, data, sizeof(Line2D));
    break;
  case ENT_SECTOR:
    render.rfunc = (RenderFunc)renderSector2D;
    dest->data = malloc(sizeof(Sector));
    sectorClone(data, dest->data);
    break;
  }

  dest->type = type;
  dest->loc = loc;
  dest->render = render;

  // *dest = (Entity){
  //     .type = type,
  //     .render = render,
  //     .loc = loc,
  // };
}
