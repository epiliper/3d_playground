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

typedef void (*RenderFunc)(void *self, Body *body, RenderInfo rinfo,
                           RenderPayload r, RenderMods *mods);

// A wrapper around data that describes how it is rendered.
typedef struct {
  RenderFunc rfunc;
  RenderInfo rinfo;
} Renderable;

typedef enum {
  ENT_CUBE,
  ENT_GRID,
} EntityType;

typedef struct {
  Body loc;
  EntityType type;
  Renderable render;
  void *data;
  uint16_t id;
} Entity;

void entityLoadFromData(void *data, uint8_t type, Body loc, Entity *dest);
#endif
