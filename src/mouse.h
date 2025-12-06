#ifndef G_MOUSE
#define G_MOUSE

#include "window.h"

// const int MOUSE_NO_PICK = 1 << 25;

#define MOUSE_NO_PICK 1 << 24

typedef struct {
  double xpos, ypos;
  float NDC[2];
  int picked;
} Mouse;

extern Mouse mouse;

void mouseInit(Window *w);
#endif
