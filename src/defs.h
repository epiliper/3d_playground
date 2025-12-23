#include <stdint.h>
#include "renderable.h"
#include "dynarray.h"

#define SECTOR_DEFAULT_VERT_CAP 10

typedef struct {
  uint16_t x, y;
} Vertex;

typedef struct {
  DynArray verts;
  uint16_t floor_height;
  uint16_t ceil_height;
} Sector;

// temporary sector we use before storing someplace
extern Sector stagingSector;

// start drawing a sector from a single vertex
void beginDrawingSector(Vertex *first, uint16_t floor_height,
                        uint16_t ceil_height);

// store the completed sector in some dest
void stopDrawingSector(Sector *dest);

void renderSector2D(Sector *s, RenderInfo rinfo, RenderPayload r,
                    RenderMods *mods);

void renderSector3D(Sector *s, RenderInfo rinfo, RenderPayload r,
                    RenderMods *mods);
