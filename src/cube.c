#include "cube.h"
#include "glad.h"

// clang-format off
// x     y     z     tex_x tex_y
float CUBE_VERTICES[120] = {
  // front (z = 0.5)
  -0.5, -0.5,  0.5,   1.0f, 1.0f,  // 0
   0.5, -0.5,  0.5,   1.0f, 0.0f,  // 1
   0.5,  0.5,  0.5,   0.0f, 0.0f,  // 2
  -0.5,  0.5,  0.5,   0.0f, 1.0f,  // 3
  
  // back (z = -0.5)
  -0.5, -0.5, -0.5,   1.0f, 1.0f,  // 4
   0.5, -0.5, -0.5,   1.0f, 0.0f,  // 5
   0.5,  0.5, -0.5,   0.0f, 0.0f,  // 6
  -0.5,  0.5, -0.5,   0.0f, 1.0f,  // 7
  
  // left (x = -0.5)
  -0.5, -0.5, -0.5,   1.0f, 1.0f,  // 8
  -0.5, -0.5,  0.5,   1.0f, 0.0f,  // 9
  -0.5,  0.5,  0.5,   0.0f, 0.0f,  // 10
  -0.5,  0.5, -0.5,   0.0f, 1.0f,  // 11
  
  // right (x = 0.5)
   0.5, -0.5, -0.5,   1.0f, 1.0f,  // 12
   0.5, -0.5,  0.5,   1.0f, 0.0f,  // 13
   0.5,  0.5,  0.5,   0.0f, 0.0f,  // 14
   0.5,  0.5, -0.5,   0.0f, 1.0f,  // 15
  
  // top (y = 0.5)
  -0.5,  0.5,  0.5,   1.0f, 1.0f, // 16
   0.5,  0.5,  0.5,   1.0f, 0.0f, // 17
   0.5,  0.5, -0.5,   0.0f, 0.0f, // 18
  -0.5,  0.5, -0.5,   0.0f, 1.0f, // 19
  
  // bottom (y = -0.5)
  -0.5, -0.5,  0.5,   1.0f, 1.0f, // 20
   0.5, -0.5,  0.5,   1.0f, 0.0f, // 21
   0.5, -0.5, -0.5,   0.0f, 0.0f, // 22
  -0.5, -0.5, -0.5,   0.0f, 1.0f, // 23
};

float CUBE_TEX[48] = {
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f, 
  0.0f, 1.0f,

  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f, 
  0.0f, 1.0f,

  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f, 
  0.0f, 1.0f,

  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f, 
  0.0f, 1.0f,

  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f, 
  0.0f, 1.0f,

  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f, 
  0.0f, 1.0f,
};

unsigned int CUBE_INDICES[36] = {
  // front
  0, 1, 2,
  0, 2, 3,
  
  // back
  5, 4, 7,
  5, 7, 6,
  
  // left
  8, 9, 10,
  8, 10, 11,
  
  // right
  12, 13, 14,
  12, 14, 15,
  
  // top
  16, 17, 18,
  16, 18, 19,
  
  // bottom
  21, 20, 23,
  21, 23, 22,
};

const char *cubeVert =
    "#version 330 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "void main() {\n"
    "gl_Position = projection * view * model * vec4(pos.xyz, 1.0);\n"
    "}";

const char *cubeFrag = "#version 330 core\n"
                       "out vec4 out_color;\n"
                       "uniform vec4 color;\n"
                       "void main() {\n"
                       "out_color = color;\n"
                       "}";


RenderInfo cubeRenderInit() {
  unsigned int vbo, vao, ebo, shader;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CUBE_INDICES), CUBE_INDICES,
               GL_STATIC_DRAW);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  shader = shaderFromCharVF(cubeVert, cubeFrag);

  RenderInfo ret = {.vao = vao, .vbo = vbo, .ebo = ebo, .shader = shader};
  return ret;
}

void cubeRender(Cube *c, Body *body, RenderInfo rinfo, RenderPayload r, RenderMods *mods) {
  glBindVertexArray(rinfo.vao);
  glUseProgram(rinfo.shader);

  float scale_x = mods != NULL ? mods->scale_x : 1.0;
  float scale_y = mods != NULL ? mods->scale_y : 1.0;
  float scale_z = mods != NULL ? mods->scale_z : 1.0;

  shaderSetMat4(rinfo.shader, "projection", *r.proj);
  shaderSetMat4(rinfo.shader, "view", *r.view);
  shaderSetVec3(rinfo.shader, "pos", body->pos);


  mat4 model;
  glm_mat4_identity(model);

  glm_translate(model, body->pos); // move to pos
  glm_rotate(model, glm_rad(body->rot[0]), (vec3){0, 1, 0});
  glm_scale(model, (vec3){ body->width * scale_x, body->height * scale_y, scale_z});
  
  shaderSetMat4(rinfo.shader, "model", model);

  if (mods != NULL && mods->color != NULL) {
    shaderSetVec4(rinfo.shader, "color", *mods->color);
    } else {
      shaderSetVec4(rinfo.shader, "color", c->color);
    }

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
