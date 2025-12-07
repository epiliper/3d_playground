#include "window.h"
#include "keybindings.h"

#include "glad.h"
#include "GLFW/glfw3.h"
#include "stdio.h"

#include "camera.h"

#include "mouse.h"

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

void getMonitorRes(GLFWmonitor *monitor, int *x, int *y) {
  const GLFWvidmode *m = glfwGetVideoMode(monitor);
  *x = m->width;
  *y = m->height;
}

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

void windowCreate(Window *dest, const char *title) {
  glfwInit();
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  if (monitor != NULL) {
    getMonitorRes(monitor, &dest->resX, &dest->resY);
  } else {
    dest->resX = DEFAULT_WINDOW_WIDTH, dest->resY = DEFAULT_WINDOW_HEIGHT;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  printf("Monitor resolution x: %d | y: %d\n", dest->resX, dest->resY);
  dest->inner = glfwCreateWindow(dest->resX, dest->resY, title, monitor, NULL);

  glfwMakeContextCurrent(dest->inner);
  glfwSwapInterval(1); // running with vsync on

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return;
  } else {
    fprintf(stderr, "Render API initialized\n");
  }

  // TODO: don't initialize keybinds here
  // ALSO: need to make a function that wraps these, since order is important.
  playerBindingsSetDefault(&playerBindings);
  playerBindingsSetCallbacks(dest->inner);

  mouseInit(dest);

  // glfwSetInputMode(dest->inner, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  fpsCameraChangeResolution(&fpsCamera, dest->resX, dest->resY);
}
bool windowShouldClose(Window *w) {
  return (bool)glfwWindowShouldClose(w->inner);
}

void windowPollPlayerInputs(Window *w) {
  for (int i = 0; i < N_PLAYER_BINDINGS; i++) {

    // we handle keystate for this in a callback elsewhere
    if (playerBindings.has_callback[i]) {
      continue;
    }

    if (glfwGetKey(w->inner, playerBindings.binds[i]) == GLFW_PRESS) {
      playerBindings.pressed[i] = 1;
    } else {
      playerBindings.pressed[i] = 0;
    }
  }
}

void windowStartFrame(Window *w) { glfwPollEvents(); }

void windowEndFrame(Window *w) { glfwSwapBuffers(w->inner); }

void windowClose(Window *w) {
  glfwDestroyWindow(w->inner);
  glfwTerminate();
}
