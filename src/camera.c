#include "camera.h"

#include <math.h>
#include <stdbool.h>

#define WORLDUP {0, 1, 0}

#define CAM_FALLBACK_POS {0, 1, 5}

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
    .pos = CAM_FALLBACK_POS,
    .sensitivity = 0.1,
};

enum FPSCameraMovement {
  FPS_CAM_LEFT,
  FPS_CAM_RIGHT,
  FPS_CAM_FORWARD,
  FPS_CAM_BACK
};

void fpsCameraMoveTo(FPSCamera *c, vec3 pos) { glm_vec3_copy(pos, c->pos); };

void fpsCameraUpdateMatrices(FPSCamera *c) {
  vec3 direction;
  glm_vec3_add(c->pos, c->front, direction);

  glm_lookat(c->pos, direction, c->up, c->view);
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

  xpos *= c->sensitivity;
  ypos *= c->sensitivity;

  c->xoffset = xpos - c->lastx;
  c->yoffset = ypos - c->lasty;

  c->lastx = xpos;
  c->lasty = ypos;

  c->yaw += c->xoffset;
  c->pitch -= c->yoffset;

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

  // printf("XPOS: %f YPOS: %f\n", xpos, ypos);
  // printf("LASTX: %d LASTY: %d\n", c->lastx, c->lasty);
  // glm_mat4_print(c->view, stdout);
}
