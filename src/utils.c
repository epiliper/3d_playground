#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

const char *readFileToEnd(const char *path, int *n) {
  FILE *f = fopen(path, "r");

  fseek(f, 0, SEEK_END);
  *n = ftell(f);
  rewind(f);

  char *ret = malloc(*n * sizeof(char *));
  int read = fread(ret, *n, 1, f);
  if (read != *n) {
    printf("file partial read: %s", path);
  }

  fclose(f);
  return ret;
}

// get a direction in world space from screen coordinates
void rayDirection(vec3 origin, int screenResX, int screenResY, double cursorX,
                  double cursorY, mat4 view, mat4 projection, vec3 dest) {

  vec4 NDC = {
      2.0f * cursorX / (float)(screenResX - 1.0f),
      1.0f - 2.0f * (cursorY / screenResY),
      -1.0f,
      1.0f,
  };

  mat4 viewinv;
  mat4 projinv;

  glm_mat4_inv(projection, projinv);
  glm_mat4_mulv(projinv, NDC, NDC); // reverse projection matrix
  NDC[2] = -1.0f;
  NDC[3] = 0.0f;

  glm_mat4_inv(view, viewinv);
  glm_mat4_mulv(viewinv, NDC, NDC); // reverse view matrix
  glm_normalize(NDC);               // and get direction.
  glm_vec3_copy(NDC, dest);
}

void castRay(vec3 origin, vec2 screenRes, vec2 cursorPos, mat4 view,
             mat4 projection, Ray *r) {
  rayDirection(origin, screenRes[0], screenRes[1], cursorPos[0], cursorPos[1],
               view, projection, r->dir);
  glm_vec3_copy(r->origin, origin);
}
