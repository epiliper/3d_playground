#ifndef G_LEVEL
#define G_LEVEL

#include <stdint.h>
// #include "renderable.h"
#include "renderer.h"

// A floor is a surface along a singular y-level that contains walls, doors, and
// misc things (props, mobs)
typedef struct {
  void *walls;
  void *doors;
  void *misc;
  uint16_t nwalls, ndoors, nmisc;
} Floor;

// A level is a container of floors
typedef struct {
  Floor *floors;
  uint16_t n_floors;
  void *skybox;
  vec3 player_spawn;
} Level;

// load a level from a file path.
void levelLoad(Level *l, const char path);

// store an in-memory level in a file path.
void levelStore(Level *l, const char path);

// Populate an array with renderable objects from the level. Can be a good place
// to do render-based sorting.
Renderable *levelCreateRenderables(Level *l, uint16_t *ndest);

#endif
