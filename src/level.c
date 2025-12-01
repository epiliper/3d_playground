// #include "level.h"
// #include "cube.c"

// // when we move objects around in the world, their new xyz values must be
// // adjusted to % LEVEL_ADJ_STEP == 0.
// //
// // I want levels to have some structure and to make snapping easier without
// // having edge-detection algorithms and other fancy shit.
// #define LEVEL_ADJ_STEP 1.0

// typedef struct {
//   vec2 min;
//   vec2 max;
// } BoundingBox;

// typedef struct {
//   vec3 pos;
//   vec4 color;
//   BoundingBox bb;
// } LevelWall;

// typedef struct {
//   vec3 pos;
//   vec4 color;
//   BoundingBox bb;
// } LevelDoor;

// void levelLoad(Level *l) {
//   Floor *f;
//   LevelWall *w;
//   LevelDoor *d;

//   for (int i = 0; i < l->n_floors; i++) {
//     f = &l->floors[i];

//     for (int j = 0; j < f->nwalls; j++) {
//       w = &f->walls[j];
//       cubeRender()
//     }
//   }
// }
