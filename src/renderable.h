#ifndef G_RENDERABLE
#define G_RENDERABLE

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

// An interface that describes and object that can
// 1. initialize its vertices and shader
// 2. render itself
typedef struct {
  void (*init)(void *self);
  void (*render)(void *self, RenderPayload r);
} RenderFuncs;

// A wrapper around data that describes how it is rendered.
typedef struct {
  RenderFuncs funcs;
  void *data;
} Renderable;

// Access RenderFuncs to draw piece of data.
void renderableDraw(Renderable *r, RenderPayload *context);

// Wrap a piece of data with the functions used to render it.
void renderableCreate(void *obj, void (*init)(RenderInfo *r),
                      void (*render)(void *self));

// ======
// Shader
// ======

// Create a shader program from vertex and fragment shader files.
unsigned int shaderFromFileVF(const char *vertfile, const char *fragfile);

// Create a shader program from in-memory vertex and fragment shader code.
unsigned int shaderFromCharVF(const char *vertcode, const char *fragCode);

void shaderSetVec4(unsigned int shader, const char *uni, vec4 dat);
void shaderSetMat4(unsigned int shader, const char *uni, mat4 dat);
void shaderSetVec3(unsigned int shader, const char *uni, vec3 dat);
void shaderSetFloat(unsigned int shader, const char *uni, float dat);

/// ========
/// Renderer
/// ========
void rendererDrawScene(Renderable *objects, int n);
#endif
