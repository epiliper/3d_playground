#include "defs.h"
#include "dynarray.h"
#include "renderable.h"
#include "string.h" // memcpy
#include "glad.h"
#include <math.h>

Sector stagingSector = {
    .ceil_height = 10,
    .floor_height = 0,
    .verts =
        (DynArray){.data = NULL, .cap = 0, .len = 0, .stride = sizeof(Vertex)}};

bool drawing_sector;

RenderInfo sectorRenderInfo = {0};

// clang-format off
const float LINE_VERTICES[6] = {
  0.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f, 
};
// clang-format off

const char *lineVert = "#version 330 core\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"layout(location = 0) in vec3 pos;\n"
"void main() {\n"
  "gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
"}";

const char *lineFrag = "#version 330 core\n"
"out vec4 out_color;\n"
"void main() {\n"
"out_color = vec4(0.0, 0.0, 1.0, 0.5);\n"
"}"
;

RenderInfo lineRenderInit() {
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

void lineRender(Line2D *line, Body *_body, RenderInfo rinfo, RenderPayload r, RenderMods *mods) {
  glBindVertexArray(rinfo.vao);
  glUseProgram(rinfo.shader);

  float scale_x = mods != NULL ? mods->scale_x : 1.0;
  float scale_y = mods != NULL ? mods->scale_x : 1.0;
  float scale_z = mods != NULL ? mods->scale_x : 1.0;

  shaderSetMat4(rinfo.shader, "projection", *r.proj);
  shaderSetMat4(rinfo.shader, "view", *r.view);
  vec3 pos = {line->v1->x, 0.0f, line->v1->y};

  mat4 model;
  glm_mat4_identity(model);
  glm_translate(model, pos);

  vec3 target_dir = {
    line->v2->x - line->v1->x,
    0.0,
    line->v2->y - line->v1->y,
    // 1.0f  // Assuming 2D, z difference is 0
  };
  
  float length = glm_vec3_norm(target_dir); // line length
  glm_normalize(target_dir); // direction
  
  vec3 original_dir = {1.0f, 0.0f, 0.0f}; // dir of our indices
  glm_normalize(original_dir);
  
  vec4 quat;
  glm_quat_from_vecs(original_dir, target_dir, quat); // calc rotation to go from indices angle to line angle
  glm_quat_rotate(model, quat, model);
  
  glm_scale_uni(model, length);
  
  shaderSetMat4(rinfo.shader, "model", model);
  glLineWidth(4.0f);
  glDrawArrays(GL_LINES, 0, 2);
}

void beginDrawingSector(Vertex *first, uint16_t floor_height,
                        uint16_t ceil_height) {

  stagingSector.ceil_height = ceil_height;
  stagingSector.floor_height = floor_height;
  DynArrayAdd(&stagingSector.verts, first);
}

int sectorAddVertex(Vertex *v) {
  bool repeat = false;

  if (&stagingSector.verts.len > 0) {
    Vertex *start;
    start = DynArrayGet(&stagingSector.verts, 0);
    // we've gone full circle. Stop
    if (fabs(start->x - v->x) <= 0.5 && fabs(start->y - v->y) <= 0.5) {
      printf("REPEAT\n");
      repeat = true;
    }
  }
  DynArrayAdd(&stagingSector.verts,  v);
  if (repeat) { return SECTOR_CYCLE; } else { return SECTOR_CONTINUE; }
}

void stopDrawingSector(Sector *dest) {
  memcpy(dest, &stagingSector, sizeof(Sector));
}

void sectorClone(Sector *src, Sector *dest) {
  memcpy(dest, src, sizeof(Sector));
  dest->verts = DynArrayClone(&src->verts);

}

void renderSector2D(Sector *s, Body *_body, RenderInfo rinfo, RenderPayload r,
                    RenderMods *mods) {
  int i = 0;
  int j = 0;
  Line2D line = {0};

  if (s->verts.len < 2) return;
  
  // Draw lines connecting consecutive vertices
  for (int i = 0; i < s->verts.len - 1; i++) {
    line.v1 = DynArrayGet(&s->verts, i);
    line.v2 = DynArrayGet(&s->verts, i + 1);
    lineRender(&line, _body, rinfo, r, mods);
  }

  // // now render the line connecting the last vertex to the first one
  // line.v1 = DynArrayGet(&s->verts, s->verts.len - 1);
  // line.v2 = DynArrayGet(&s->verts, 0);


  // lineRender(&line, _body, rinfo, r, mods);
}

void renderSector3D(Sector *s, RenderInfo rinfo, RenderPayload r,
                    RenderMods *mods) {}
