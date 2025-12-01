#include "dummy.h"
#include "camera.h"
#include "glad.h"
#include "keybindings.h"
#include "cube.h"
#include "renderable.h"

Cube testCube = {
    .pos = {0, 0, -5},
    .color = {1, 1, 0, 0.5},
    .height = 5,
    .width = 10,
};

void testScenePrepare() {
  rendererInitWithCapacity(1);
  rendererAddItem(&testCube, 0, 0);
}

void testSceneProcessInput() {
#define CMOVE 0.1
  if (KBTN_DOWN(P_LEFT)) {
    fpsCameraMove(FPS_CAM_LEFT, CMOVE, &fpsCamera);
  }
  if (KBTN_DOWN(P_RIGHT)) {
    fpsCameraMove(FPS_CAM_RIGHT, CMOVE, &fpsCamera);
  }
  if (KBTN_DOWN(P_FORW)) {
    fpsCameraMove(FPS_CAM_FORWARD, CMOVE, &fpsCamera);
  }
  if (KBTN_DOWN(P_BACK)) {
    fpsCameraMove(FPS_CAM_BACK, CMOVE, &fpsCamera);
  }
}

void testSceneRender() {
  testSceneProcessInput();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  RenderPayload r = {.proj = &fpsCamera.projection, .view = &fpsCamera.view};

  rendererDrawAll(r);
}
