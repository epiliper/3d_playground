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
  N_PLAYER_BINDINGS
} PlayerBinding;

// Using enums for lookup here. will probably bite me in the ass
//
//
// This is a shared state of keys that we can modify directly from platform
// keypress callbacks.
typedef struct {
  int binds[N_PLAYER_BINDINGS];
  short pressed[N_PLAYER_BINDINGS];
} PlayerBindings;

extern PlayerBindings playerBindings;

#define KBTN_DOWN(button) playerBindings.pressed[button]

// Set the bindings to their defaults/fallbacks
void playerBindingsSetDefault(PlayerBindings *p);

// Change a key binding for a particular action
void playerBindingsChangeBindings(PlayerBindings *p);
#endif
