#ifndef G_UTILS
#define G_UTILS

#include "cglm/cglm.h"
/// Read a file's contents to end and store in a freshly-allocated string.
const char *readFileToEnd(const char *path, int *len);

typedef struct {
  vec3 origin, dir;
} Ray;

// Given screen coordinates and a point on that screen, return the position in
// worldspace.
void castRay(vec3 origin, vec2 screenRes, vec2 cursorPos, mat4 view,
             mat4 projection, Ray *r);

#endif
