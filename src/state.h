#ifndef G_STATE
#define G_STATE

typedef enum {
  APP_START,
  APP_PLAY,
  APP_LOAD,
  MENU_SETTINGS,
  MENU_PAUSE,
  MENU_INV,
  MENU_EDIT,
  MENU_QUIT,
} ApplicationState;

extern ApplicationState appState;

#endif
