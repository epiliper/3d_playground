#ifndef G_APP
#define G_APP

#define APP_TITLE "editor"

#include "window.h"

typedef struct {
  Window window;
} Application;

extern Application APP;

// initialize shit
void applicationStart();

// run said shit
void applicationRun();

// event loop for entire app
void applicationEventLoop();

#endif
