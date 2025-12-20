#ifndef G_MOUSE
#define G_MOUSE

#include "dynarray.h"
#include "window.h"
#include "cglm/cglm.h"

// const int MOUSE_NO_PICK = 1 << 25;
#define MOUSE_MAX_PICK 16

typedef struct {
  double xpos, ypos;
  double lastx, lasty;
  double rotatex, rotatey;
  bool left_dwn, right_dwn, mid_dwn, moved, rotating;
  float NDC[2];
  DynArray picked;
} Mouse;

typedef struct {
  vec3 origin;
  vec3 pos;
  vec3 dir;
  vec3 dir_inv;
} ray;

extern Mouse mouse;

void mouseInit(Window *w);

// Use ray-cast picking to select objects
void mousePick3D();
void mousePick2D();
#endif
