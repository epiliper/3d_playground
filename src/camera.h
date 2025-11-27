#ifndef G_CAMERA
#define G_CAMERA

#include "cglm/cglm.h"

// forward-dec

// A first-person camera with a perspective matrix that can update with mouse
// drag.
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

typedef enum {
  FPS_CAM_LEFT,
  FPS_CAM_RIGHT,
  FPS_CAM_FORWARD,
  FPS_CAM_BACK
} FPSCameraMovement;

extern FPSCamera fpsCamera;

// move camera to 3D position.
void fpsCameraMoveTo(FPSCamera *c, vec3 pos);

// nudge camera's position by direction and amount
void fpsCameraMove(FPSCameraMovement direction, float amount, FPSCamera *c);

// update camera view and projection matrices given new state.
void fpsCameraUpdateMatrices(FPSCamera *c);

// adjust camera to updated resolution and recompute matrices
void fpsCameraChangeResolution(FPSCamera *c, float x, float y);

// adjust camera to new field of view and recompute matrices
void fpsCameraChangeFOV(FPSCamera *c, float fov);

// update internal camera state given a mouse drag event and recompute matrices
void fpsCameraPan(double xpos, double ypos, FPSCamera *c);

#endif
