#ifndef G_MOUSE
#define G_MOUSE

#include "window.h"

typedef struct {
  double xpos, ypos;
} Mouse;

extern Mouse mouse;

void mouseInit(Window *w);
#endif
