#include "renderable.h"
#include "entity.h"
#include "glad.h"
#include "utils.h"
#include "string.h" // memset
#include "mouse.h"
#include "app.h"
#include "camera.h"
#include <stdbool.h>
#include "keybindings.h"
#include "defs.h"

int success;
char infoLog[512];

static float ROT_90_RAD;
vec3 obj_rot_start;

bool rotating = false;
bool just_rotated;

bool scaling = false;
bool just_scaled;

bool copy = false;
int to_copy;

bool delete = false;
int to_delete;

// shorthand for getting a ray's direction in 3d space from mouse
#define CAST_RAY_DIR(dest)                                                     \
  castRay(fpsCamera.pos, (vec2){APP.window.resX, APP.window.resY},             \
          (vec2){mouse.xpos, mouse.ypos}, *renderPayload.view,                 \
          *renderPayload.proj, dest, true);

// shorthand for getting ray's position from mouse
#define CAST_RAY_POS(dest)                                                     \
  castRay(fpsCamera.pos, (vec2){APP.window.resX, APP.window.resY},             \
          (vec2){mouse.xpos, mouse.ypos}, *renderPayload.view,                 \
          *renderPayload.proj, dest, false);

// shorthand for rendering RenderItem
#define RENDER(e, rinfo, rpayload, mods)                                       \
  e->render.rfunc(e->data, &e->loc, rinfo, rpayload, mods);

//
// Renderable
//

void renderableDraw(Renderable *r, RenderPayload *context) {
  // r->RenderFunc(r->data, *context);
}

void renderableCreate(void *obj, void (*init)(RenderInfo *r),
                      void (*render)(void *self));

//
// Renderer
//

// add an entity, update its ID, and check if we need to initialize its
// rendering functions
void rendererAddEntity(Renderer *r, Entity *e) {
  e->id = renderer3D.ents.len;

  // render data not yet initialized
  if (r->rinfo_man[e->type].vao == 0) {
    printf("Initializing rinfo for type %d\n", e->type);
    RenderInfo rinfo = (e->render.rinitfunc)();
    r->rinfo_man[e->type] = rinfo;
  } else {
    printf("Render info for type %d already initialized\n", e->type);
  }

  e->render.rinfo = e->type;
  e->render.init = RENDER_INIT;
  DynArrayAdd(&renderer3D.ents, e);
}

// remove an entity by id (linear search)
void rendererDeleteEntity(Renderer *r, int id) {
  Entity *e;

  for (int i = 0; i < r->ents.len; i++) {
    e = DynArrayGet(&r->ents, i);
    if (e->id == id) {
      DynArraySwapRemove(&r->ents, i, NULL);
      return;
    }
  }

  printf("Attempting to delete entity that isn't there by id: %d\n", id);
}

Renderer renderer3D = {.ents = {0}, .track_picking = true};
Renderer renderer2D = {.ents = {0}, .track_picking = true};

const int RENDER_SLOT_EMPTY = 1 << 10;

void rendererInitWithCapacity(Renderer *r, int cap) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ROT_90_RAD = glm_rad(15.0f);

  if (r->ents.len != 0) {
    DynArrayDestroy(&r->ents);
  }

  // TODO: Handle errors here.
  // renderer.ents = malloc(sizeof(Entity) * cap);
  r->ents = DynArrayInit(cap, Entity);
  r->texman.textures = DynArrayInit(cap, unsigned int);
}

void rendererDrawAll3D(RenderPayload renderPayload) {
  Entity *e;
  uint32_t picked = 0;
  RenderInfo rinfo;

  GLenum err = 0;
  for (int i = 0; i < renderer3D.ents.len; i++) {
    e = DynArrayGet(&renderer3D.ents, i);
    rinfo = renderer3D.rinfo_man[e->type];
    RENDER(e, rinfo, renderPayload, NULL);
    // item->e.render.rfunc(item->e.data, &item->e.loc, rinfo, renderPayload,
    //                      NULL);
  }

  rendererPickingPhase(&pickingSystem, &renderer3D.ents, renderPayload);

  pickingRequestPick(&pickingSystem, mouse.xpos, mouse.ypos);

  bool has_pick = pickingGetAsync(&pickingSystem, &picked);

  if (mouse.mid_dwn) {
    fpsCameraPan(mouse.xpos, mouse.ypos, &fpsCamera);
  } else {
    fpsCamera.firstmouse = true;
  }

  if (mouse.left_dwn) {
    // clear if we already have items selected
    //
    // TODO: add shift toggle here to select multiple
    if (mouse.picked.len > 0) {
      DynArrayClear(&mouse.picked);
    } else {
      if (has_pick && picked != 0) {
        int id = picked - 1;
        DynArrayAdd(&mouse.picked, &id);
      };
    }

    mouse.left_dwn = false;
  }

  // case 1: we have an object selected and are moving it.
  if (mouse.picked.len > 0) {
    vec3 rotate = {0};
    vec3 scale = {0};

    bool just_scaled = false;
    bool just_rotated = false;

    // just started rotating
    if (KBTN_DOWN(E_EDIT_ROTATE) && !rotating) {
      rotating = true;
      mouse.rotatex = mouse.xpos;
      mouse.rotatey = mouse.ypos;
      just_rotated = true;

      // still rotating
    } else if (rotating) {

      float change_y = (mouse.ypos - mouse.rotatey) / APP.window.resY * 360 * 2;
      float change_x = (mouse.xpos - mouse.rotatex) / APP.window.resX * 360 * 2;

      change_x = CLAMP_TO_MULTIPLE(change_x, 30.0f);
      change_y = CLAMP_TO_MULTIPLE(change_y, 30.0f);

      just_rotated = false;
      glm_vec3_copy((vec3){change_x, change_y, 0}, rotate);

      // we just released the rotate button, so calculate the mouse
      // offset from the start of the rotation
      if (!KBTN_DOWN(E_EDIT_ROTATE)) {
        rotating = false;
      }
    }

    // just started rotating
    if (KBTN_DOWN(E_EDIT_SCALE) && !scaling) {
      scaling = true;
      mouse.rotatex = mouse.xpos;
      mouse.rotatey = mouse.ypos;
      just_scaled = true;

      // still scaling
    } else if (scaling) {

      float change_y =
          (mouse.ypos - mouse.rotatey) / APP.window.resY * 100 * 0.3;
      float change_x =
          (mouse.xpos - mouse.rotatex) / APP.window.resX * 100 * 0.3;

      change_x = -CLAMP_TO_MULTIPLE(change_x, 1.0f);
      change_y = -CLAMP_TO_MULTIPLE(change_y, 1.0f);

      just_scaled = false;
      glm_vec3_copy((vec3){change_x, change_y, 0}, scale);

      // we just released the rotate button, so calculate the mouse
      // offset from the start of the rotation
      if (!KBTN_DOWN(E_EDIT_SCALE)) {
        scaling = false;
      }
    }

    // this entire branch needs to be a function... It's going to
    // get big real fast
    Ray ray;
    CAST_RAY_DIR(&ray);
    // castRay(fpsCamera.pos, (vec2){APP.window.resX, APP.window.resY},
    //         (vec2){mouse.xpos, mouse.ypos}, *renderPayload.view,
    //         *renderPayload.proj, &ray);

    Body b;
    for (int i = 0; i < mouse.picked.len; i++) {
      vec3 pos;
      int *id;

      id = DynArrayGet(&mouse.picked, i); // get the id of the picked object.
      e = DynArrayGet(&renderer3D.ents, *id); // get the object itself.

      rinfo = renderer3D.rinfo_man[e->type];

      if (KBTN_DOWN(E_EDIT_COPY) && KBTN_DOWN(C_CONTROL)) {
        copy = true;
        to_copy = i;
        KBTN_RELEASE(E_EDIT_COPY);

      } else if (KBTN_DOWN(E_EDIT_DELETE)) {
        delete = true;
        to_delete = i;
      }

      RenderMods m = {
          .color = &(vec4){1.0, 0.33, 0.33, 0.22},
          .scale_x = 1.05,
          .scale_y = 1.05,
          .scale_z = 1.05,

      };

      glm_vec3_copy(e->loc.pos, pos);

      if (rotating) {
        if (just_rotated)
          glm_vec3_copy(e->loc.rot, obj_rot_start);

        glm_vec3_add(obj_rot_start, rotate, e->loc.rot);
      } else if (scaling) {
        if (just_scaled) {
          obj_rot_start[0] = e->loc.width;
          obj_rot_start[1] = e->loc.height;
        }

        e->loc.width = obj_rot_start[0] + scale[0];
        e->loc.height = obj_rot_start[1] + scale[1];
      } else {

        float distance = glm_vec3_distance(ray.origin, pos);

        vec3 drag_pos;
        drag_pos[0] = ray.origin[0] + ray.dir[0] * distance;
        drag_pos[1] = ray.origin[1] + ray.dir[1] * distance;
        drag_pos[2] = ray.origin[2] + ray.dir[2] * distance;

        levelSanitizePosition(drag_pos);

        glm_vec3_copy(drag_pos, e->loc.pos);
      }

      RENDER(e, rinfo, renderPayload, &m);
    }

    if (copy) {
      for (int i = 0; i < mouse.picked.len; i++) {
        int *id;
        Entity *e;
        id = DynArrayGet(&mouse.picked, i);
        e = DynArrayGet(&renderer3D.ents, *id);

        rendererAddEntity(&renderer3D, e);
      }
    }
    copy = false;

    if (delete) {
      for (int i = 0; i < mouse.picked.len; i++) {
        int *id;
        id = DynArrayGet(&mouse.picked, i);
        rendererDeleteEntity(&renderer3D, *id);
      }

      // update mouse selection so we aren't referencing deleted
      // elements.
      DynArrayClear(&mouse.picked);
      delete = false;

      picked = 0;
    }
  }

  // case 2: we are actively looking for things to pick
  else if (has_pick && picked != 0) {
    RenderMods m = {
        .color = &(vec4){0.33, 0.33, 1.0, 0.22},
        .scale_x = 1.05,
        .scale_y = 1.05,
        .scale_z = 1.05,

    };

    e = DynArrayGet(&renderer3D.ents, picked - 1);
    rinfo = renderer3D.rinfo_man[e->type];

    if (e != NULL) {
      RENDER(e, rinfo, renderPayload, &m);
      // item->render.rfunc(item->data, &item->loc, item->render.rinfo,
      //                    renderPayload, &m);
    }
  };
}

void rendererDrawAll2D(RenderPayload renderPayload) {
  Vertex v;
  Ray r;
  if (mouse.left_dwn) {
    CAST_RAY_POS(&r);
    printf("MOUSE CAST: %f %f\n", r.dir[0], r.dir[2]);
    v.x = r.dir[0];
    v.y = r.dir[2]; // z coordinate is y when we're top-down 2D.
    if (drawing_sector) {
      if (sectorAddVertex(&v) == SECTOR_CYCLE) {
        printf("Cycle detected\n");
        drawing_sector = false;
      }
    } else {
      beginDrawingSector(&v, 10, 20);
    }

    mouse.left_dwn = false;
  }

  renderSector2D(&stagingSector, NULL, sectorRenderInfo, renderPayload, NULL);
}

//
// GLSL shader utility functions
//

unsigned int shaderFromFileVF(const char *vertfile, const char *fragfile) {
  unsigned int frag, vert;
  const char *src_string;

  int sourcelen;
  src_string = readFileToEnd(vertfile, &sourcelen);
  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &src_string, NULL);
  glCompileShader(vert);

  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling vertex shader: %s\n", infoLog);
  }
  free((char *)src_string);

  src_string = readFileToEnd(fragfile, &sourcelen);
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &src_string, NULL);
  glCompileShader(frag);

  glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling frag shader: %s\n", infoLog);
  }

  free((char *)src_string);

  unsigned int shader = glCreateProgram();
  glAttachShader(shader, vert);
  glAttachShader(shader, frag);

  glLinkProgram(shader);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return shader;
}

unsigned int shaderFromCharVF(const char *vertcode, const char *fragcode) {
  unsigned int frag, vert;

  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vertcode, NULL);
  glCompileShader(vert);

  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling vertex shader: %s\n", infoLog);
  }

  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &fragcode, NULL);
  glCompileShader(frag);

  glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling frag shader: %s\n", infoLog);
  }

  unsigned int shader = glCreateProgram();
  glAttachShader(shader, vert);
  glAttachShader(shader, frag);

  glLinkProgram(shader);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return shader;
}

void shaderSetVec4(unsigned int shader, const char *uni, vec4 dat) {
  GLint loc = glGetUniformLocation(shader, uni);
  // if (loc == -1) {
  //   printf("Failed to find uniform %s\n", uni);
  // }
  glUseProgram(shader);
  glUniform4fv(loc, 1, dat);
}

void shaderSetMat4(unsigned int shader, const char *uni, mat4 dat) {
  GLint loc = glGetUniformLocation(shader, uni);
  if (loc == -1) {
    printf("Failed to find uniform %s\n", uni);
  }
  glUseProgram(shader);
  glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)dat);
}

void shaderSetVec3(unsigned int shader, const char *uni, vec3 dat) {
  GLint loc = glGetUniformLocation(shader, uni);
  if (loc == -1) {
    printf("Failed to find uniform %s\n", uni);
  }
  glUseProgram(shader);
  glUniform3fv(loc, 1, dat);
}

void shaderSetFloat(unsigned int shader, const char *uni, float dat) {
  GLint loc = glGetUniformLocation(shader, uni);
  if (loc == -1) {
    printf("Failed to find uniform %s\n", uni);
  }
  glUseProgram(shader);
  glUniform1f(loc, dat);
}

void shaderSetUnsignedInt(unsigned int shader, const char *uni,
                          unsigned int dat) {
  GLint loc = glGetUniformLocation(shader, uni);
  if (loc == -1) {
    printf("Failed to find uniform %s\n", uni);
  }
  glUseProgram(shader);
  glUniform1ui(loc, dat);
}

///
/// PICKING
///
///
/// Using OGLDev's approach: write to a texture in a second
/// framebuffer, read it during rendering the primary buffer to see
/// if our mouse position overlaps with an object index written to
/// the buffer.

// Shaders for painting a buffer with object IDs.

const char *pickingVert =
    "#version 330\n"
    "layout (location = 0) in vec3 position;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "void main() {\n"
    " gl_Position = projection * view * model * vec4(position, "
    "1.0);\n"
    "}\n";

const char *pickingFrag =
    "#version 330\n"
    "uniform uint objectIndex;\n"
    "uniform vec4 color;\n" // this is a placeholder to avoid opengl error 1281
    "out uint fragColor;\n"
    "void main() {\n"
    " fragColor = objectIndex;\n"
    "}";

PickingSystem pickingSystem;

void pickingTextureInit(PickingSystem *t, int screenResX, int screenResY) {
  glGenFramebuffers(1, &t->fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);

  // TEXTURE for metadata
  glGenTextures(1, &t->picktex);
  glBindTexture(GL_TEXTURE_2D, t->picktex);
  // pay attention to types
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, screenResX, screenResY, 0,
               GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         t->picktex, 0);

  // TEXTURE for depth buffer
  glGenTextures(1, &t->depthtex);
  glBindTexture(GL_TEXTURE_2D, t->depthtex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenResX, screenResY, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         t->depthtex, 0);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    printf("Failed to create picking buffer!\n");
    exit(1);
  }

  // reset to default state.
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void pickingShaderInit(PickingSystem *t) {
  t->shader = shaderFromCharVF(pickingVert, pickingFrag);
}

void pickingPBOInit(PickingSystem *t) {
  glGenBuffers(2, t->pbos);
  for (int i = 0; i < 2; i++) {
    glBindBuffer(GL_PIXEL_PACK_BUFFER, t->pbos[i]);
    glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(uint32_t), NULL, GL_STREAM_READ);
  }

  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

bool pickingGetAsync(PickingSystem *t, uint32_t *dest) {
  if (!t->awaiting_pick) {
    return false;
  }

  // Read from the OTHER PBO (the one from the previous frame)
  int readIdx = 1 - t->pbo_idx;
  glBindBuffer(GL_PIXEL_PACK_BUFFER, t->pbos[readIdx]);

  // Check if the data is ready without blocking
  GLint is_mapped;
  glGetBufferParameteriv(GL_PIXEL_PACK_BUFFER, GL_BUFFER_MAPPED, &is_mapped);

  if (!is_mapped) {
    // Try non-blocking map
    uint32_t *ptr = (uint32_t *)glMapBufferRange(
        GL_PIXEL_PACK_BUFFER, 0, sizeof(uint32_t), GL_MAP_READ_BIT);

    if (ptr) {
      *dest = *ptr;
      glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
      glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
      t->awaiting_pick = false;
      return true;
    }
  }

  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  return false;
}

void pickingRequestPick(PickingSystem *t, int mouseX, int mouseY) {
  t->awaiting_pick = true;
  t->pbo_idx = 1 - t->pbo_idx;

  glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);

  // Bind current PBO and start async read
  glBindBuffer(GL_PIXEL_PACK_BUFFER, t->pbos[t->pbo_idx]);
  glReadPixels(mouseX, APP.window.resY - mouseY, 1, 1, GL_RED_INTEGER,
               GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rendererPickingPhase(PickingSystem *t, DynArray *ents,
                          RenderPayload renderPayload) {
  glBindFramebuffer(GL_FRAMEBUFFER, t->fbo);
  glUseProgram(t->shader);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Entity *e;
  RenderInfo rinfo;

  for (int i = 0; i < ents->len; i++) {
    shaderSetUnsignedInt(t->shader, "objectIndex", i + 1);
    e = DynArrayGet(ents, i);
    // e = &ents[i];
    //
    rinfo = renderer3D.rinfo_man[e->type];
    rinfo.shader = t->shader;

    RENDER(e, rinfo, renderPayload, NULL);
    // e->render.rfunc(e->data, &e->loc, rinfo, renderPayload, NULL);
  }

  // reset
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void pickingSystemInit(int screenResX, int screenResY) {
  pickingTextureInit(&pickingSystem, screenResX, screenResY);
  pickingShaderInit(&pickingSystem);
  pickingPBOInit(&pickingSystem);
}
