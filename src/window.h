#ifndef G_WINDOW
#define G_WINDOW

#include <stdbool.h>

typedef struct {
  int monitor, resX, resY;
  void *inner;
} Window;

// Create a window given a title and a Window struct. This will attempt to
// retrieve the primary monitor's resolution and render fullscreen, falling back
// to a small res if that fails.
void windowCreate(Window *dest, const char *title);

// Check for input events that correspond to player actions.
void windowPollPlayerInputs(Window *w);

// Check for anything
void windowPoll(Window *w);

// Window termination procedure
void windowClose(Window *w);

bool windowShouldClose(Window *w);
#endif
