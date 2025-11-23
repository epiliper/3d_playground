#include "keybindings.h"
#include "window.h"
#include <stdbool.h>

#define APP_TITLE "editor"

typedef struct {
  Window window;
} Application;

Application APP = {0};

void applicationStart() { windowCreate(&APP.window, APP_TITLE); }

void applicationRun() {
  while (!windowShouldClose(&APP.window)) {
    windowPoll(&APP.window);
  }

  windowClose(&APP.window);
}
