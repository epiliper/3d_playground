#include "window.h"
#include "keybindings.h"

#include "glad.h"
#include "GLFW/glfw3.h"
#include "stdio.h"

#include "camera.h"

#include "mouse.h"

#define WIN_FALLBACK_X 640
#define WIN_FALLBACK_Y 480

// get monitor pixel X and Y without being befuddled by retina displays
void getTrueMonitorRes(GLFWmonitor *monitor, int *x, int *y) {
  const GLFWvidmode *m = glfwGetVideoMode(monitor);
  float scaleX, scaleY;
  glfwGetMonitorContentScale(monitor, &scaleX, &scaleY);
  *x = m->width * scaleX;
  *y = m->height * scaleY;
}

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

void windowCreate(Window *dest, const char *title) {
  glfwInit();
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();

  if (monitor != NULL) {
    getTrueMonitorRes(monitor, &dest->resX, &dest->resY);
  } else {
    dest->resX = WIN_FALLBACK_X, dest->resY = WIN_FALLBACK_Y;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  dest->inner = glfwCreateWindow(dest->resX, dest->resY, title, monitor, NULL);

  glfwGetWindowSize(dest->inner, &dest->resX, &dest->resY);
  glfwGetFramebufferSize(dest->inner, &dest->fbX, &dest->fbY);

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

  glViewport(0, 0, dest->fbX, dest->fbY);
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
