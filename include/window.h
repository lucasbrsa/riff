/* @TODO don't break when wm closes window */
/* @TODO support X11 winapi wayland and cocoa */

#ifndef _WINDOW_H
#define _WINDOW_H

#include "os.h"

#ifndef OS_LINUX
#error UNSUPPORTED_OS
#endif

/* generate a new X11 server linked window and return it's ID */
int window_X11_create(const char* title, int w, int h);

/* poll event on window */
unsigned window_X11_poll_event(int win_id);

/* free memory and close the window */
void window_X11_close(int win_id);

/* free memory and close all windows */
void window_X11_close_all(void);

#endif
