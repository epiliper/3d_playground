#include "mouse.h"
#include "GLFW/glfw3.h"
#include "stdio.h"
#include "camera.h"

Mouse mouse;

void mouseUpdatePos(GLFWwindow *window, double xpos, double ypos) {
  mouse.xpos = xpos;
  mouse.ypos = ypos;
  fpsCameraPan(xpos, ypos, &fpsCamera);
};

void mouseInit(Window *w) {
  glfwSetCursorPosCallback(w->inner, mouseUpdatePos);
}
