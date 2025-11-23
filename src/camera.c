#include "cglm/cglm.h"
#include <math.h>
#include <stdbool.h>

#define WORLDUP {0, 1, 0}

typedef struct {
  bool firstmouse;
  int lastx, lasty;
  int xoffset, yoffset;
  float yaw, pitch;
  float sensitivity;
  int width, height;
  mat4 view;
  mat4 projection;
  vec3 front, up, right, pos;
  float fov;
} FPSCamera;

FPSCamera fpsCamera = {
    .firstmouse = true,
    .lastx = 0,
    .lasty = 0,
    .xoffset = 0,
    .yoffset = 0,
    .yaw = 0,
    .pitch = 0,
    .width = 0,
    .height = 0,
    .fov = 45.0,
};

enum FPSCameraMovement {
  FPS_CAM_LEFT,
  FPS_CAM_RIGHT,
  FPS_CAM_FORWARD,
  FPS_CAM_BACK
};

void fpsCameraMoveTo(FPSCamera *c, vec3 pos) { glm_vec3_copy(pos, c->pos); };

void fpsCameraUpdateMatrices(FPSCamera *c) {
  glm_lookat(c->front, c->pos, c->up, c->view);
  glm_perspective(glm_rad(c->fov), (float)c->width / c->height, 0.1f, 100.0f,
                  c->projection);
}

void fpsCameraChangeResolution(FPSCamera *c, float x, float y) {
  c->width = x;
  c->height = y;
  fpsCameraUpdateMatrices(c);
}

void fpsCameraChangeFOV(FPSCamera *c, float fov) {
  c->fov = fov;
  fpsCameraUpdateMatrices(c);
}

void fpsCameraMove(int direction, float amount, FPSCamera *c) {
  switch (direction) {
  case FPS_CAM_FORWARD:
    c->pos[2] += amount;
    break;
  case FPS_CAM_BACK:
    c->pos[2] -= amount;
    break;
  case FPS_CAM_LEFT:
    c->pos[0] -= amount;
    break;
  case FPS_CAM_RIGHT:
    c->pos[0] += amount;
    break;
  }

  fpsCameraUpdateMatrices(c);
}

/// Recalculate camera pitch, yaw, and matrices given new mouse cursor x and
/// y coordinates.
void fpsCameraPan(double xpos, double ypos, FPSCamera *c) {
  if (c->firstmouse) {
    c->lastx = xpos;
    c->lasty = ypos;
    c->firstmouse = false;
  };

  c->xoffset = (xpos - c->lastx) * c->sensitivity;
  c->yoffset = (ypos - c->lasty) * c->sensitivity;

  c->lastx = xpos;
  c->lasty = ypos;

  c->yaw += c->xoffset;
  c->pitch += c->yoffset;

  // gimbal lock is unwelcome
  c->pitch = c->pitch > 89.0f ? 89.0f : c->pitch;
  c->pitch = c->pitch < -89.0f ? -89.0f : c->pitch;

  // compute left, right and up directions for view matrix
  c->front[0] = cos(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
  c->front[1] = sin(glm_rad(c->pitch));
  c->front[2] = sin(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
  glm_normalize(c->front);

  glm_cross(c->front, (vec3)WORLDUP, c->right);
  glm_normalize(c->right);

  glm_cross(c->right, c->front, c->up);
  glm_normalize(c->up);

  fpsCameraUpdateMatrices(c);
}
