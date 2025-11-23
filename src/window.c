#include "window.h"
#include "keybindings.h"

#include "GLFW/glfw3.h"
#include "stdio.h"

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

  printf("Monitor resolution x: %d | y: %d\n", dest->resX, dest->resY);
  dest->inner = glfwCreateWindow(dest->resX, dest->resY, title, monitor, NULL);

  glfwMakeContextCurrent(dest->inner);
  glfwSwapInterval(1); // running with vsync on

  // TODO: don't initialize keybinds here
  playerBindingsSetDefault(&playerBindings);
}

bool windowShouldClose(Window *w) {
  return (bool)glfwWindowShouldClose(w->inner);
}

void windowPollPlayerInputs(Window *w) {
  for (int i = 0; i < N_PLAYER_BINDINGS; i++) {
    if (glfwGetKey(w->inner, playerBindings.binds[i]) == GLFW_PRESS) {
      playerBindings.pressed[i] = 1;
      printf("KEY CODE %d pressed\n", playerBindings.binds[i]);
    } else {
      playerBindings.pressed[i] = 0;
    }
  }
}

void windowPoll(Window *w) {
  glfwPollEvents();
  windowPollPlayerInputs(w);
  glfwSwapBuffers(w->inner);
}

void windowClose(Window *w) {
  glfwDestroyWindow(w->inner);
  glfwTerminate();
}
