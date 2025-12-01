#include "renderable.h"
#include "glad.h"
#include "utils.h"
#include "cube.h"
#include "string.h" // memset

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

Renderer renderer = {.items = NULL, .rinfo = NULL, .n_items = 0, .n_rinfo = 0};

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
  }
  }

  // TODO: handle textures.

  bucket = shape;

  Renderable r = {.data = item, .rfunc = rfunc, .rinfo = rinfo};
  renderer.items[renderer.n_items] = r;
  renderer.n_items++;
}

void rendererDrawAll(RenderPayload renderPayload) {
  Renderable *r;
  for (int i = 0; i < renderer.n_items; i++) {
    r = &renderer.items[i];
    r->rfunc(r->data, r->rinfo, renderPayload);
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
