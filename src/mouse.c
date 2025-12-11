#include "mouse.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "stdio.h"
#include "camera.h"

Mouse mouse;

void mouseUpdatePosFPS(GLFWwindow *window, double xpos, double ypos) {
  mouse.lastx = mouse.xpos;
  mouse.lasty = mouse.ypos;
  mouse.xpos = xpos;
  mouse.ypos = ypos;
  // mouse.picked[0] = 0;
  // mouse.npick = 1;
  fpsCameraPan(xpos, ypos, &fpsCamera);
};

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    mouse.left_dwn = true;
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    mouse.right_dwn = true;
}

void mouseInit(Window *w) {
  glfwSetCursorPosCallback(w->inner, mouseUpdatePosFPS);
  glfwSetMouseButtonCallback(w->inner, mouseButtonCallback);
  glfwSetInputMode(w->inner, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

//
// RAY CASTING FROM MOUSE
//
void getNormalizedDeviceCoordinates(int resX, int resY, float mouseX,
                                    float mouseY, vec2 dest) {
  float newx = (2.0f * mouseX) / (float)resX - 1.0f;
  float newy = 1.0f - 2.0f * (mouseY / (float)resY);
  dest[0] = newx;
  dest[1] = newy;
};

void clipCoordsToEyeSpace(vec4 clip_coords, mat4 projection, vec4 dest) {
  mat4 inv;
  glm_mat4_inv(projection, inv);

  vec4 eyecoords;
  glm_mat4_mulv(inv, clip_coords, eyecoords);
  dest[0] = eyecoords[0];
  dest[1] = eyecoords[1];
  dest[2] = -1.0f;
  dest[3] = 0.0f;

  // vec_print("Eye coordinates", dest);
}

/// Reverse the eye coordinates to world space
void eyeCoordsToWorldSpace(vec4 eye_coords, mat4 view, vec3 dirdest,
                           vec3 posdest) {
  mat4 inv;
  glm_mat4_inv(view, inv);

  vec4 world_coords;
  glm_mat4_mulv(inv, eye_coords, world_coords);

  glm_vec3_copy(world_coords, posdest);
  glm_vec3_copy(world_coords, dirdest);
  glm_normalize(dirdest);

  // vec_print("World coords", dest);
}

void calculateRayDirection(int width, int height, float x, float y,
                           mat4 projection, mat4 view, vec3 dirdest,
                           vec3 posdest) {
  // normalized device coordinates
  vec2 normalized;
  getNormalizedDeviceCoordinates(width, height, x, y, normalized);

  // clip space
  vec4 clip_space = {normalized[0], normalized[1], -1.0f, 1.0f};

  // eye coordinates
  vec4 eye_coords;
  clipCoordsToEyeSpace(clip_space, projection, eye_coords);

  // world coordinates: A direction into world space.
  eyeCoordsToWorldSpace(eye_coords, view, dirdest, posdest);
}

ray getRay(int width, int height, float x, float y, mat4 projection, mat4 view,
           vec3 origin) {
  ray r = {0};

  calculateRayDirection(width, height, x, y, projection, view, r.dir, r.pos);

  r.dir_inv[0] = 1 / r.dir[0];
  r.dir_inv[1] = 1 / r.dir[1];
  r.dir_inv[2] = 1 / r.dir[2];

  glm_vec3_copy(origin, r.origin);

  return r;
}
