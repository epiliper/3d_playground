#include "GLFW/glfw3.h"
#include "string.h" // memset

#include "keybindings.h"
#include "stdio.h"

PlayerBindings playerBindings = {0};

void keyAddCallback(PlayerBindings *p, int key, int action) {
  KeyCallbacksToCheck *cb = &p->callbacks;

  if (cb->n == N_PLAYER_BINDINGS)
    return;

  cb->keys[cb->n] = key;
  cb->handles[cb->n] = &p->pressed[action];
  cb->n++;
}

// define a keybinding that fires with every key press/repeat
#define KSET_BIND(bindings, action, key)                                       \
  bindings->binds[action] = key;                                               \
  bindings->has_callback[action] = 0;

// override default key detection with a callback func called on keypress.
#define KSET_BIND_CALLBACK(bindings, action, key)                              \
  bindings->binds[action] = key;                                               \
  bindings->has_callback[action] = 1;                                          \
  keyAddCallback(bindings, key, action);

// ==============================
// Default/fallback keybindings.
// ==============================

// movement
static const int K_LEFT = GLFW_KEY_A;
static const int K_RIGHT = GLFW_KEY_D;
static const int K_FORW = GLFW_KEY_W;
static const int K_BACK = GLFW_KEY_S;
static const int K_SPRINT = GLFW_KEY_RIGHT_SHIFT;

// mouse
static const int K_SHOOT = GLFW_MOUSE_BUTTON_LEFT;
static const int K_AIM = GLFW_MOUSE_BUTTON_RIGHT;

// misc
static const int K_INV = GLFW_KEY_I;
static const int K_PAUSE = GLFW_KEY_ESCAPE;
static const int K_CAM_ISO = GLFW_KEY_Q;

// editor
static const int K_EDIT_ROTATE = GLFW_KEY_R;

void playerBindingsCheckCallbacks(GLFWwindow *w, int key, int scancode,
                                  int action, int mods) {
  for (int i = 0; i < playerBindings.callbacks.n; i++) {
    if (key == playerBindings.callbacks.keys[i] && action == GLFW_PRESS) {
      *playerBindings.callbacks.handles[i] = 1;
    } else {
      *playerBindings.callbacks.handles[i] = 0;
    }
  }
}

void playerBindingsSetCallbacks(void *w) {
  glfwSetKeyCallback((GLFWwindow *)w, playerBindingsCheckCallbacks);
}

void playerBindingsSetDefault(PlayerBindings *p) {
  KSET_BIND(p, P_LEFT, K_LEFT);
  KSET_BIND(p, P_RIGHT, K_RIGHT);
  KSET_BIND(p, P_FORW, K_FORW);
  KSET_BIND(p, P_BACK, K_BACK);

  KSET_BIND(p, P_SPRINT, K_SPRINT);

  KSET_BIND(p, P_SHOOT, K_SHOOT);
  KSET_BIND(p, P_AIM, K_AIM);

  KSET_BIND(p, P_INV, K_INV);
  KSET_BIND(p, P_PAUSE, K_PAUSE);

  KSET_BIND_CALLBACK(p, P_ISO_TOGGLE, K_CAM_ISO);
  KSET_BIND_CALLBACK(p, E_EDIT_ROTATE, K_EDIT_ROTATE);
}

// TODO: update this to detect key collision logic.
void playerBindingsChangeBinding(PlayerBindings *p, int action, int binding) {
  KSET_BIND(p, action, binding);
}
