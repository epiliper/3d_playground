#ifndef G_RENDERABLE
#define G_RENDERABLE

#include "cglm/cglm.h"
#include "entity.h"

// Access RenderFuncs to draw piece of data.
void renderableDraw(Renderable *r, RenderPayload *context);

// Wrap a piece of data with the functions used to render it.
void renderableCreate(void *obj, void (*init)(RenderInfo *r),
                      void (*render)(void *self));

// ========
// Renderer
// ========
// To avoid loading the same assets twice (textures, vertices), we keep track of
// ones we already loaded using two identifiers: one for shape(mesh) and another
// for texture.
//
// When iterating over level items, we attempt to inititalize render info for
// each one, unless it has already been initialized.
//
// The renderer contains a list of objects that can be rendered, with the
// rendering functions and info assigned during the loading phase, before
// anything is rendered.
typedef struct {
  Entity *ents;
  int n;
  bool track_picking;
} Renderer;

typedef struct {
} RenderInfoManager;

typedef struct {

} TextureManager;

extern Renderer renderer;

// const int RENDERER_SLOT_EMPTY = 1 << 4;
#define RENDERER_SLOT_EMPTY 1 >> 4

// add an item to the renderer given a shape and tex id.
void rendererAddItem(void *item, int shape, int tex);

// delete an item given its id.
void rendererDeleteItem(int id);

// create a renderer with space for at least @cap renderables (not necessarily
// distinct shape/tex ids)
void rendererInitWithCapacity(int cap);

void rendererDrawAll(RenderPayload renderPayload);

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

// ==============
// Picking system
// ==============

typedef struct {
  unsigned int fbo, picktex, depthtex, shader;
  unsigned int pbos[2];
  unsigned int pbo_idx;
  mat4 temp_model;
  bool awaiting_pick;
} PickingSystem;

extern PickingSystem pickingSystem;

void pickingSystemInit(int screenResX, int screenResY);

// query picking data from the picking buffer.
void pickingRequestPick(PickingSystem *t, int mouseX, int mouseY);

bool pickingGetAsync(PickingSystem *t, uint32_t *dest);

// draw all renderables to a picking framebuffer.
void rendererPickingPhase(PickingSystem *t, Entity *ents, int n,
                          RenderPayload renderPayload);

#endif
