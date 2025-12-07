#include "renderable.h"
#include "glad.h"
#include "utils.h"
#include "cube.h"
#include "string.h" // memset
#include "mouse.h"
#include "app.h"

int success;
char infoLog[512];

//
// Renderable
//

void renderableDraw(Renderable *r, RenderPayload *context) {
  // r->RenderFunc(r->data, *context);
}

void renderableCreate(void *obj, void (*init)(RenderInfo *r),
                      void (*render)(void *self));

//
// Renderer
//

Renderer renderer = {.items = NULL,
                     .rinfo = NULL,
                     .n_items = 0,
                     .n_rinfo = 0,
                     .track_picking = true};

const int RENDER_SLOT_EMPTY = 1 << 10;

void rendererInitWithCapacity(int cap) {
  if (renderer.items != NULL)
    free(renderer.items);
  if (renderer.rinfo != NULL)
    free(renderer.rinfo);

  renderer.n_items = 0, renderer.n_rinfo = 0;

  // TODO: Handle errors here.
  renderer.items = malloc(sizeof(Renderable) * cap);
  renderer.rinfo = malloc(sizeof(RenderInfo) * cap);

  for (int i = 0; i < cap; i++) {
    renderer.rinfo[i].vao = RENDER_SLOT_EMPTY;
  }
}

void rendererAddItem(void *item, int shape, int tex) {
  int bucket = 0;
  RenderInfo rinfo;
  RenderFunc rfunc;
  ModelFunc mfunc;

  switch (shape) {
  case 0: {
    Cube *c = item;
    if (renderer.rinfo[0].vao == RENDER_SLOT_EMPTY) {
      rinfo = cubeRenderInit();
      renderer.rinfo[0] = rinfo;
      renderer.n_rinfo++;
    }

    rinfo = renderer.rinfo[0];
    rfunc = (RenderFunc)cubeRender;
    mfunc = (ModelFunc)cubeUpdateModel;
  }
  }

  // TODO: handle textures.

  bucket = shape;

  // MAKE THIS INTO A FUNCTION TO AVOID NULL POINTERS!!!!!!!!!!!!!!!!
  Renderable r = {.data = item, .rfunc = rfunc, .mfunc = mfunc, .rinfo = rinfo};
  renderer.items[renderer.n_items] = r;
  renderer.n_items++;
}

void rendererDrawAll(RenderPayload renderPayload) {
  Renderable *r;

  for (int i = 0; i < renderer.n_items; i++) {
    r = &renderer.items[i];
    r->rfunc(r->data, r->rinfo, renderPayload, NULL);
  }

  if (renderer.track_picking) {
    uint32_t picked = 0;
    rendererPickingPhase(&pickingSystem, renderer.items, renderer.n_items,
                         renderPayload);

    pickingRequestPick(&pickingSystem, mouse.xpos, mouse.ypos);
    if (pickingGetAsync(&pickingSystem, &picked) && picked != 0) {
      RenderMods m = {.color = &(vec4){0.33, 0.33, 1.0, 1.0},
                      .scale_x = 1.05,
                      .scale_y = 1.05};

      r = &renderer.items[picked - 1];
      r->rfunc(r->data, r->rinfo, renderPayload, &m);
    };
  }
}

//
// GLSL shader utility functions
//

unsigned int shaderFromFileVF(const char *vertfile, const char *fragfile) {
  unsigned int frag, vert;
  const char *src_string;

  int sourcelen;
  src_string = readFileToEnd(vertfile, &sourcelen);
  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &src_string, NULL);
  glCompileShader(vert);

  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling vertex shader: %s\n", infoLog);
  }
  free((char *)src_string);

  src_string = readFileToEnd(fragfile, &sourcelen);
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &src_string, NULL);
  glCompileShader(frag);

  glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling frag shader: %s\n", infoLog);
  }

  free((char *)src_string);

  unsigned int shader = glCreateProgram();
  glAttachShader(shader, vert);
  glAttachShader(shader, frag);

  glLinkProgram(shader);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return shader;
}

unsigned int shaderFromCharVF(const char *vertcode, const char *fragcode) {
  unsigned int frag, vert;

  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertcode, NULL);
  glCompileShader(vert);

  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling vertex shader: %s\n", infoLog);
  }

  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragcode, NULL);
  glCompileShader(frag);

  glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling frag shader: %s\n", infoLog);
  }

  unsigned int shader = glCreateProgram();
  glAttachShader(shader, vert);
  glAttachShader(shader, frag);

  glLinkProgram(shader);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return shader;
}

void shaderSetVec4(unsigned int shader, const char *uni, vec4 dat) {
  unsigned int loc = glGetUniformLocation(shader, uni);
  glUseProgram(shader);
  glUniform4fv(loc, 1, dat);
}

void shaderSetMat4(unsigned int shader, const char *uni, mat4 dat) {
  unsigned int loc = glGetUniformLocation(shader, uni);
  glUseProgram(shader);
  glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)dat);
}

void shaderSetVec3(unsigned int shader, const char *uni, vec3 dat) {
  unsigned int loc = glGetUniformLocation(shader, uni);
  glUseProgram(shader);
  glUniform3fv(loc, 1, dat);
}

void shaderSetFloat(unsigned int shader, const char *uni, float dat) {
  unsigned int loc = glGetUniformLocation(shader, uni);
  glUseProgram(shader);
  glUniform1f(loc, dat);
}

void shaderSetUnsignedInt(unsigned int shader, const char *uni,
                          unsigned int dat) {
  unsigned int loc = glGetUniformLocation(shader, uni);
  glUseProgram(shader);
  glUniform1ui(loc, dat);
}

///
/// PICKING
///
///
/// Using OGLDev's approach: write to a texture in a second framebuffer, read it
/// during rendering the primary buffer to see if our mouse position overlaps
/// with an object index written to the buffer.

// Shaders for painting a buffer with object IDs.

const char *pickingVert =
    "#version 330\n"
    "layout (location = 0) in vec3 position;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main() {\n"
    " gl_Position = projection * view * model * vec4(position, 1.0);\n"
    "}\n";

const char *pickingFrag = "#version 330\n"
                          "uniform uint objectIndex;\n"
                          "out uint fragColor;\n"
                          "void main() {\n"
                          " fragColor = objectIndex;\n"
                          "}";

PickingSystem pickingSystem;

void pickingTextureInit(PickingSystem *t, int screenResX, int screenResY) {
  glGenFramebuffers(1, &t->fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);

  // TEXTURE for metadata
  glGenTextures(1, &t->picktex);
  glBindTexture(GL_TEXTURE_2D, t->picktex);
  // pay attention to types
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, screenResX, screenResY, 0,
               GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         t->picktex, 0);

  // TEXTURE for depth buffer
  glGenTextures(1, &t->depthtex);
  glBindTexture(GL_TEXTURE_2D, t->depthtex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenResX, screenResY, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         t->depthtex, 0);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    printf("Failed to create picking buffer!\n");
    exit(1);
  }

  // reset to default state.
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void pickingShaderInit(PickingSystem *t) {
  t->shader = shaderFromCharVF(pickingVert, pickingFrag);
}

void pickingPBOInit(PickingSystem *t) {
  glGenBuffers(2, t->pbos);
  for (int i = 0; i < 2; i++) {
    glBindBuffer(GL_PIXEL_PACK_BUFFER, t->pbos[i]);
    glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(uint32_t), NULL, GL_STREAM_READ);
  }

  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

bool pickingGetAsync(PickingSystem *t, uint32_t *dest) {
  if (!t->awaiting_pick) {
    return false;
  }

  // Read from the OTHER PBO (the one from the previous frame)
  int readIdx = 1 - t->pbo_idx;
  glBindBuffer(GL_PIXEL_PACK_BUFFER, t->pbos[readIdx]);

  // Check if the data is ready without blocking
  GLint is_mapped;
  glGetBufferParameteriv(GL_PIXEL_PACK_BUFFER, GL_BUFFER_MAPPED, &is_mapped);

  if (!is_mapped) {
    // Try non-blocking map
    uint32_t *ptr = (uint32_t *)glMapBufferRange(
        GL_PIXEL_PACK_BUFFER, 0, sizeof(uint32_t), GL_MAP_READ_BIT);

    if (ptr) {
      *dest = *ptr;
      glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
      glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
      t->awaiting_pick = false;
      return true;
    }
  }

  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  return false;
}

void pickingRequestPick(PickingSystem *t, int mouseX, int mouseY) {
  t->awaiting_pick = true;
  t->pbo_idx = 1 - t->pbo_idx;

  glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);

  // Bind current PBO and start async read
  glBindBuffer(GL_PIXEL_PACK_BUFFER, t->pbos[t->pbo_idx]);
  glReadPixels(mouseX, APP.window.resY - mouseY, 1, 1, GL_RED_INTEGER,
               GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rendererPickingPhase(PickingSystem *t, Renderable *r, int n,
                          RenderPayload renderPayload) {
  glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);
  glUseProgram(t->shader);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Renderable *item;
  RenderInfo rinfo;

  for (int i = 0; i < n; i++) {
    shaderSetUnsignedInt(t->shader, "objectIndex", i + 1);
    item = &r[i];
    rinfo = item->rinfo;
    rinfo.shader = t->shader;

    item->rfunc(item->data, rinfo, renderPayload, NULL);
  }

  // reset
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void pickingSystemInit(int screenResX, int screenResY) {
  pickingTextureInit(&pickingSystem, screenResX, screenResY);
  pickingShaderInit(&pickingSystem);
  pickingPBOInit(&pickingSystem);
}
