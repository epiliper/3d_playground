#include "cube.h"
#include "glad.h"

// clang-format off
// x     y     z     tex_x tex_y
float CUBE_VERTICES[120] = {
  // front (z = 0.5)
  -0.5, -0.5,  0.1,   1.0f, 1.0f,  // 0
   0.5, -0.5,  0.1,   1.0f, 0.0f,  // 1
   0.5,  0.5,  0.1,   0.0f, 0.0f,  // 2
  -0.5,  0.5,  0.1,   0.0f, 1.0f,  // 3
  
  // back (z = -0.5)
  -0.5, -0.5, -0.1,   1.0f, 1.0f,  // 4
   0.5, -0.5, -0.1,   1.0f, 0.0f,  // 5
   0.5,  0.5, -0.1,   0.0f, 0.0f,  // 6
  -0.5,  0.5, -0.1,   0.0f, 1.0f,  // 7
  
  // left (x = -0.5)
  -0.5, -0.5, -0.1,   1.0f, 1.0f,  // 8
  -0.5, -0.5,  0.1,   1.0f, 0.0f,  // 9
  -0.5,  0.5,  0.1,   0.0f, 0.0f,  // 10
  -0.5,  0.5, -0.1,   0.0f, 1.0f,  // 11
  
  // right (x = 0.5)
   0.5, -0.5, -0.1,   1.0f, 1.0f,  // 12
   0.5, -0.5,  0.1,   1.0f, 0.0f,  // 13
   0.5,  0.5,  0.1,   0.0f, 0.0f,  // 14
   0.5,  0.5, -0.1,   0.0f, 1.0f,  // 15
  
  // top (y = 0.5)
  -0.5,  0.5,  0.1,   1.0f, 1.0f, // 16
   0.5,  0.5,  0.1,   1.0f, 0.0f, // 17
   0.5,  0.5, -0.1,   0.0f, 0.0f, // 18
  -0.5,  0.5, -0.1,   0.0f, 1.0f, // 19
  
  // bottom (y = -0.5)
  -0.5, -0.5,  0.1,   1.0f, 1.0f, // 20
   0.5, -0.5,  0.1,   1.0f, 0.0f, // 21
   0.5, -0.5, -0.1,   0.0f, 0.0f, // 22
  -0.5, -0.5, -0.1,   0.0f, 1.0f, // 23
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

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
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

  mat4 model;
  glm_mat4_identity(model);

  glm_translate(model, body->pos); // move to pos
  glm_rotate(model, glm_rad(body->rot[0]), (vec3){0, 1, 0});
  glm_rotate(model, glm_rad(body->rot[1]), (vec3){1, 0, 0});
  glm_scale(model, (vec3){ body->width * scale_x, body->height * scale_y, scale_z});
  
  shaderSetMat4(rinfo.shader, "model", model);

  if (mods != NULL && mods->color != NULL) {
    shaderSetVec4(rinfo.shader, "color", *mods->color);
    } else {
      shaderSetVec4(rinfo.shader, "color", c->color);
    }

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

///
/// GRID
/// 

// clang-format off
float SQUARE_VERTICES[12] = {
  -0.5, 0.5,  0.1, 
  0.5,  0.5,  0.1,
  0.5,  -0.5, 0.1,
  -0.5, -0.5, 0.1
};

unsigned int SQUARE_INDICES[6] = {
  0, 1, 2, 2, 3, 0
};
// clang-format on

const char *gridVert = "#version 330 core\n"
                       "layout (location = 0) in vec3 pos;\n"
                       "uniform mat4 view;\n"
                       "uniform mat4 projection;\n"
                       "uniform mat4 model;\n"

                       "out vec4 world_pos;\n"

                       "void main() {\n"
                       "world_pos = model * vec4(pos.xyz, 1.0);\n"
                       "gl_Position = projection * view * world_pos;\n"
                       "}";

const char *gridFrag =
    "#version 330 core\n"
    "out vec4 out_color;\n"
    "in vec4 world_pos;\n"
    "uniform float linewidth;\n"
    "\n"
    "float grid_at_scale(vec2 pos, float scale) {\n"
    "    vec2 grid = abs(fract(pos / scale));\n"
    "    float line = min(grid.x, grid.y);\n"
    "    float line_derivative = fwidth(line);\n"
    "    float linewidth = 0.02;\n"
    "    return 1.0 - smoothstep(linewidth - line_derivative * 3.0, \n"
    "                            linewidth + line_derivative * 3.0, \n"
    "                            line);\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec2 world_derivative = fwidth(world_pos.xz);\n"
    "    float max_derivative = max(world_derivative.x, world_derivative.y);\n"
    "    \n"
    "    // Fine grid (1 unit spacing)\n"
    "    float grid_fine = grid_at_scale(world_pos.xz, 1.0);\n"
    "    // Fade out when zoomed out\n"
    "    float fade_fine = 1.0 - smoothstep(0.3, 1.0, max_derivative);\n"
    "    grid_fine *= fade_fine;\n"
    "    \n"
    "    // Coarse grid (10 unit spacing) - thicker lines\n"
    "    float grid_coarse = grid_at_scale(world_pos.xz, 10.0);\n"
    "    // Fade in as fine grid fades out, fade out when very far\n"
    "    float fade_coarse = smoothstep(0.5, 2.0, max_derivative) * \n"
    "                        (1.0 - smoothstep(3.0, 10.0, max_derivative));\n"
    "    grid_coarse *= fade_coarse;\n"
    "    \n"
    "    // Combine grids\n"
    "    float grid_strength = max(grid_fine, grid_coarse * 0.7);\n"
    "    \n"
    "    vec3 background = vec3(0.2);\n"
    "    vec3 line_color = vec3(0.8);\n"
    "    vec3 color = mix(background, line_color, grid_strength);\n"
    "    \n"
    "    out_color = vec4(color, 1.0);\n"
    "}";

// clang-format on
//

RenderInfo gridRenderInit() {
  unsigned int vbo, vao, ebo, shader;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SQUARE_INDICES), SQUARE_INDICES,
               GL_STATIC_DRAW);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(SQUARE_VERTICES), SQUARE_VERTICES,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  shader = shaderFromCharVF(gridVert, gridFrag);

  RenderInfo ret = {.vao = vao, .vbo = vbo, .ebo = ebo, .shader = shader};
  return ret;
}

void gridRender(Grid *g, Body *body, RenderInfo rinfo, RenderPayload r,
                RenderMods *mods) {
  glBindVertexArray(rinfo.vao);
  glUseProgram(rinfo.shader);

  float scale_x = mods != NULL ? mods->scale_x : 1.0;
  float scale_y = mods != NULL ? mods->scale_y : 1.0;
  float scale_z = mods != NULL ? mods->scale_z : 1.0;

  shaderSetMat4(rinfo.shader, "projection", *r.proj);
  shaderSetMat4(rinfo.shader, "view", *r.view);

  mat4 model;
  glm_mat4_identity(model);

  glm_translate(model, body->pos); // move to pos
  glm_rotate(model, glm_rad(body->rot[0]), (vec3){1, 0, 0});
  glm_rotate(model, glm_rad(body->rot[1]), (vec3){0, 1, 0});
  glm_scale(model,
            (vec3){body->width * scale_x, body->height * scale_y, scale_z});

  shaderSetMat4(rinfo.shader, "model", model);

  // shaderSetVec4(rinfo.shader, "line_color", g->line_color);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
