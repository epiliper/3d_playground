#include "defs.h"
#include "renderable.h"
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

void lineRender(Line2D line, Body *body, RenderInfo rinfo, RenderPayload r, RenderMods *mods) {
  glBindVertexArray(rinfo.vao);
  glUseProgram(rinfo.shader);

  float scale_x = mods != NULL ? mods->scale_x : 1.0;
  float scale_y = mods != NULL ? mods->scale_x : 1.0;
  float scale_z = mods != NULL ? mods->scale_x : 1.0;

  shaderSetMat4(rinfo.shader, "projection", *r.proj);
  shaderSetMat4(rinfo.shader, "view", *r.proj);

  mat4 model;
  glm_mat4_identity(model);

  glm_translate(model, (vec3){line.v1->x, line.v1->y, 1});

  // calculate quaternation to represent angle between v1 and v2
  vec3 original_dir;
  vec4 quat;

  // TODO: this needs to be cached; this is just the original vector between the two line points we upload to VBO at runtime init.
  glm_vec2_sub((vec3){LINE_VERTICES[3], LINE_VERTICES[4], LINE_VERTICES[5]}, (vec3){LINE_VERTICES[0], LINE_VERTICES[1], LINE_VERTICES[2]}, original_dir);

  glm_normalize(original_dir);
  glm_quat_from_vecs(original_dir, (vec3){line.v2->x, line.v2->y, 1}, quat);
  glm_quat_rotate(model, quat, model); // perform the quaternion rotation
  
  glm_scale_uni(model, 50); // thicc lines
  shaderSetMat4(rinfo.shader, "model", model);
  glDrawArrays(GL_LINES, 0, 2);
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
