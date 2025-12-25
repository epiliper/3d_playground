#ifndef G_ENT
#define G_ENT
#include "cglm/cglm.h"

/// ====================
/// Renderable INTERFACE
/// ====================

typedef struct {
  unsigned int vao, vbo, ebo, shader;
} RenderInfo;

typedef struct {
  mat4 *proj, *view;
} RenderPayload;

typedef struct {
  vec4 *color;
  float scale_x, scale_y, scale_z;
} RenderMods;

// Basic features of a 3D shape in the world
typedef struct {
  vec3 pos;
  vec3 rot;
  float height, width;
} Body;

// Function to render the entity
typedef void (*RenderFunc)(void *self, Body *body, RenderInfo rinfo,
                           RenderPayload r, RenderMods *mods);

// Function initializing VAO, VBO, EBO, and shader.
typedef RenderInfo (*RenderInitFunc)();

typedef void (*CloneFunc)(void *self, void *dest);

enum { RENDER_INIT, RENDER_UNINIT };

// A wrapper around data that describes how it is rendered.
//
// the render info is can already initialized by the time the entity is loaded,
// in which case a handle is returned to the entity instead of cloning VBOS,
// VAOs, etc...
typedef struct {
  RenderFunc rfunc;
  union {
    RenderInitFunc rinitfunc;
    unsigned int rinfo;
  };
  int init;
} Renderable;

typedef enum {
  ENT_CUBE,
  ENT_GRID,
  ENT_LINE,
  ENT_SECTOR,
  ENT_N_TYPES,
} EntityType;

typedef struct {
  Body loc;
  EntityType type;
  Renderable render;
  void *data;
  CloneFunc clonefunc;
  uint16_t id;
} Entity;

// load an entity from generic data and assign its function pointers.
void entityLoadFromData(void *data, uint8_t type, Body loc, Entity *dest);
#endif
