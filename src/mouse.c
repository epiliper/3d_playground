#include "mouse.h"
#include "glad.h"
#include "GLFW/glfw3.h"

Mouse mouse;

void mouseUpdatePosFPS(GLFWwindow *window, double xpos, double ypos) {
  mouse.lastx = mouse.xpos;
  mouse.lasty = mouse.ypos;
  mouse.xpos = xpos;
  mouse.ypos = ypos;
  mouse.moved = true;
  // fpsCameraPan(xpos, ypos, &fpsCamera);
};

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    mouse.left_dwn = true;
  if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    mouse.mid_dwn = action == GLFW_PRESS;
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    mouse.right_dwn = true;
}

void mouseInit(Window *w) {
  glfwSetCursorPosCallback(w->inner, mouseUpdatePosFPS);
  glfwSetMouseButtonCallback(w->inner, mouseButtonCallback);
  mouse.picked = DynArrayInit(MOUSE_MAX_PICK, int);
  mouse.left_dwn = false;
  mouse.mid_dwn = false;
  mouse.right_dwn = false;
  mouse.moved = false;
  mouse.rotating = false;
  // glfwSetInputMode(w->inner, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
  glfwSetInputMode(w->inner, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
