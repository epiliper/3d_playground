#include "camera.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define WORLDUP {0, 1, 0}

#define CAM_FALLBACK_POS {0, 1, 0}

FPSCamera fpsCamera = {.firstmouse = true,
                       .lastx = 0,
                       .lasty = 0,
                       .xoffset = 0,
                       .yoffset = 0,
                       .yaw = -90,
                       .pitch = 0,
                       .width = 0,
                       .height = 0,
                       .fov = 45.0,
                       .pos = CAM_FALLBACK_POS,
                       .sensitivity = 0.1,
                       .front = {-0.5, 0, -1}};

void fpsCameraMoveTo(FPSCamera *c, vec3 pos) { glm_vec3_copy(pos, c->pos); };

void fpsCameraViewMatrix(FPSCamera *c) {
  // compute left, right and up directions for view matrix
  c->front[0] = cos(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
  c->front[1] = sin(glm_rad(c->pitch));
  c->front[2] = sin(glm_rad(c->yaw)) * cos(glm_rad(c->pitch));
  glm_normalize(c->front);

  glm_cross(c->front, (vec3)WORLDUP, c->right);
  glm_normalize(c->right);

  glm_cross(c->right, c->front, c->up);
  glm_normalize(c->up);

  vec3 direction;
  glm_vec3_add(c->pos, c->front, direction);

  glm_lookat(c->pos, direction, c->up, c->view);
}

void fpsCameraProjectionMatrix(FPSCamera *c) {
  glm_perspective(glm_rad(c->fov), (float)c->width / c->height, 0.1f, 100.0f,
                  c->projection);
}

void fpsCameraUpdateMatrices(FPSCamera *c) {
  fpsCameraViewMatrix(c);
  fpsCameraProjectionMatrix(c);
}

void fpsCameraChangeResolution(FPSCamera *c, float x, float y) {
  c->width = x;
  c->height = y;
  c->firstmouse = true;
  fpsCameraUpdateMatrices(c);
}

void fpsCameraChangeFOV(FPSCamera *c, float fov) {
  c->fov = fov;
  fpsCameraUpdateMatrices(c);
}

void fpsCameraMove(FPSCameraMovement direction, float amount, FPSCamera *c) {
  float velocity = 0.1;
  vec3 movement = {0, 0, 0};

  switch (direction) {
  case FPS_CAM_FORWARD:
    glm_vec3_add(movement, (vec3){c->front[0], 0.0, c->front[2]}, movement);
    break;
  case FPS_CAM_BACK:
    glm_vec3_sub(movement, (vec3){c->front[0], 0.0, c->front[2]}, movement);
    break;
  case FPS_CAM_LEFT:
    glm_vec3_sub(movement, c->right, movement);
    break;
  case FPS_CAM_RIGHT:
    glm_vec3_add(movement, c->right, movement);
    break;
  }

  glm_normalize(movement);
  glm_vec3_scale(movement, velocity, movement);
  glm_vec3_add(c->pos, movement, c->pos);

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
  c->pitch -= c->yoffset;

  // gimbal lock is unwelcome
  c->pitch = c->pitch > 89.0f ? 89.0f : c->pitch;
  c->pitch = c->pitch < -89.0f ? -89.0f : c->pitch;

  fpsCameraUpdateMatrices(c);
}
