#ifndef G_KEYBIND
#define G_KEYBIND
// ==================================
// Keybinds needed for player control
// ==================================

typedef enum {
  P_LEFT,
  P_RIGHT,
  P_FORW,
  P_BACK,
  P_SPRINT,
  P_SHOOT,
  P_AIM,
  P_INV,
  P_PAUSE,
  P_ISO_TOGGLE,
  E_EDIT_ROTATE,
  N_PLAYER_BINDINGS
} PlayerBinding;

// A list of keys that we can check with callbacks instead of per-frame polling.
// each handle points to the key-associated "pressed" value.
typedef struct {
  int keys[N_PLAYER_BINDINGS];
  short *handles[N_PLAYER_BINDINGS];
  int n;
} KeyCallbacksToCheck;

// Using enums for lookup here. will probably bite me in the ass
//
//
// This is a shared state of keys that we can modify directly from platform
// keypress callbacks.
//
// if we need to detect each key just once per press (no repeat signal),
// then we can provide a callback that lets us do that.
typedef struct {
  int binds[N_PLAYER_BINDINGS];
  short pressed[N_PLAYER_BINDINGS];
  short has_callback[N_PLAYER_BINDINGS];
  KeyCallbacksToCheck callbacks;
} PlayerBindings;

extern PlayerBindings playerBindings;

#define KBTN_DOWN(button) playerBindings.pressed[button]
#define KBTN_RELEASE(button) playerBindings.pressed[button] = 0

// Set the bindings to their defaults/fallbacks
void playerBindingsSetDefault(PlayerBindings *p);

// Change a key binding for a particular action
void playerBindingsChangeBindings(PlayerBindings *p);

// attach key press callbacks to a GLFW window
void playerBindingsSetCallbacks(void *w);
#endif
