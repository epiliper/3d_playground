#include "mouse.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "stdio.h"
#include "camera.h"

Mouse mouse;

void getNormalizedDeviceCoordinates(int resX, int resY, float mouseX,
                                    float mouseY, vec2 dest) {
  float newx = (2.0f * mouseX) / (float)resX - 1.0f;
  float newy = 1.0f - 2.0f * (mouseY / (float)resY);
  dest[0] = newx;
  dest[1] = newy;
};

void mouseUpdatePos(GLFWwindow *window, double xpos, double ypos) {
  mouse.xpos = xpos;
  mouse.ypos = ypos;
  mouse.picked = MOUSE_NO_PICK;
  fpsCameraPan(xpos, ypos, &fpsCamera);
};

void mouseInit(Window *w) {
  glfwSetCursorPosCallback(w->inner, mouseUpdatePos);
}
