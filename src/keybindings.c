#include "GLFW/glfw3.h"
#include "string.h" // memset

#include "keybindings.h"

PlayerBindings playerBindings = {0};

#define KSET_BIND(bindings, action, key) bindings->binds[action] = key;

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
}

// TODO: update this to detect key collision logic.
void playerBindingsChangeBinding(PlayerBindings *p, int action, int binding) {
  KSET_BIND(p, action, binding);
}
