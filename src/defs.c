#include "defs.h"
#include "string.h" // memcpy
#include "glad.h"

Sector stagingSector = {0};

// clang-format off
const float LINE_VERTICES[6] = {
  0.0f, 0.0f, 0.0f,
  1.0f, 1.0f, 1.0f

};
// clang-format off

const char *lineVert = "version 330 core\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"uniform vec3 pos;\n"
"void main() {\n"
  "gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
"}";

const char *lineFrag = "version 330 core\n"
"uniform vec4 line_color;\n"
"out vec4 out_color;\n"
"void main() {\n"
"out_color = line_color;\n"
"}"
;

RenderInfo lineRendererInit() {
  unsigned int vao, vbo, shader;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(LINE_VERTICES), LINE_VERTICES, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  shader = shaderFromCharVF(lineVert, lineFrag);
  RenderInfo ret = {.vao = vao, .vbo = vbo, .ebo = 0, .shader = shader};
  return ret;
}

void lineRender() {
}

void beginDrawingSector(Vertex *first, uint16_t floor_height,
                        uint16_t ceil_height) {

  stagingSector.ceil_height = ceil_height;
  stagingSector.floor_height = floor_height;
  DynArrayAdd(&stagingSector.verts, first);
}

void stopDrawingSector(Sector *dest) {
  memcpy(dest, &stagingSector, sizeof(Sector));
}

void renderSector2D(Sector *s, RenderInfo rinfo, RenderPayload r,
                    RenderMods *mods) {
  int i = 0;
  int j = 0;

  Vertex *v1, *v2;
  for (j = 1; i < s->verts.len; i++) {
    DynArrayGet(&s->verts, i, v1);
    DynArrayGet(&s->verts, j, v2);

    i++;
  }
}

void renderSector3D(Sector *s, RenderInfo rinfo, RenderPayload r,
                    RenderMods *mods) {}
