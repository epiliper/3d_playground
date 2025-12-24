#include "dummy.h"
#include "camera.h"
#include "glad.h"
#include "keybindings.h"
#include "cube.h"
#include "renderable.h"
#include "app.h"
#include "entity.h"
#include "defs.h"

Cube testCube = {
    .color = {1, 1, 0, 1.0},
};

Grid testGrid = {.line_color = {1, 1, 0, 1.0}};

Line2D testLine = {.v1 = &(Vertex){.x = 10, .y = 2},
                   .v2 = &(Vertex){.x = 20, .y = 2}};

void testScenePrepare() {
  rendererInitWithCapacity(2);
  pickingSystemInit(APP.window.resX, APP.window.resY);
  Entity e;
  entityLoadFromData(
      &testCube, ENT_CUBE,
      (Body){.pos = {0, 0, -5}, .rot = {0, 0, 0}, .height = 5, .width = 10},
      &e);
  rendererAddEntity(&e);

  entityLoadFromData(
      &testGrid, ENT_GRID,
      (Body){
          .pos = {0, -1, -5}, .rot = {90, 0, 0}, .height = 1000, .width = 1000},
      &e);

  rendererAddEntity(&e);

  entityLoadFromData(&testLine, ENT_LINE, (Body){0}, &e);
  rendererAddEntity(&e);
  // DynArrayAdd(&renderer.ents, &e);
  // rendererAddItem(&testCube, 0, 0);

  glEnable(GL_DEPTH_TEST);
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
  if (KBTN_DOWN(P_ISO_TOGGLE)) {
    fpsCameraToggleIso(&fpsCamera);
    KBTN_RELEASE(P_ISO_TOGGLE);
  }
}

void testSceneRender() {
  testSceneProcessInput();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  RenderPayload r = {.proj = &fpsCamera.projection, .view = &fpsCamera.view};

  rendererDrawAll(r);
}
