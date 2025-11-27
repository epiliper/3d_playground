#include "app.h"

#include "window.h"

#include "camera.h"
#include "dummy.h"
#include "state.h"
#include "stdio.h"

// =================
// TEST RENDER SCENE
// =================
void dummyLevelLoad() {
  printf("Loading level\n");
  testScenePrepare();
}

void dummyLevelRender() { testSceneRender(); }

// crude example of how a level would be.
void applicationEventLoop() {
  switch (appState) {
  case APP_LOAD:
    dummyLevelLoad();
    appState = APP_PLAY;
    break;

  case APP_PLAY:
    dummyLevelRender();
    break;

  default:
    break;
  }
}

Application APP = {0};

void applicationStart() { windowCreate(&APP.window, APP_TITLE); }

void applicationRun() {
  while (!windowShouldClose(&APP.window)) {
    // FRAME START
    windowStartFrame(&APP.window);
    windowPollPlayerInputs(&APP.window);

    // INSERT EVENT LOOP HERE.
    applicationEventLoop();

    // FRAME END
    windowEndFrame(&APP.window);
  }

  windowClose(&APP.window);
}
