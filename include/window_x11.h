/* https://github.com/gamedevtech/X11OpenGLWindow */
/* @TODO don't break when wm closes window */
/* @TODO buffer window size/pos/.. and then only change when a change event occurs */
/* @TODO MITSHM */

/* should not be included directly (most of the time) */
/* instead use the window wrapper window.h */
#ifndef _WINDOW_X11_H
#define _WINDOW_X11_H

#include "error.h"
#include "window.h"

#include <stdbool.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct {
	Window win;
	Visual* vis;
	Colormap cmap;
	GC gc;

	bool activated;
} window_data_s;

/* the error callback */
int window_X11_error_handler(Display* dpy, XErrorEvent* err);

/* open a connection with the X11 server, and initialize library globals */
void window_X11_open_connection(void);

/* close the X11 connection */
void window_X11_close_connection(void);

/* create a window */
status_t window_X11_init(const char* title, unsigned width, unsigned height);

/* close a single window instance */
void window_X11_close(void);

/* poll events on window, don't wait if there are none */
void window_X11_event_poll(void);

/* poll events on window, and wait if there are none */
void window_X11_event_wait(void);

/* poll events on window, and wait if there are none */
void window_X11_event_pause(float t);

/* has the window been closed, set with window_close() */
bool window_X11_get_closed(void);

/* get/set size */
/* get/set fullscreen */
/* get/set title */
/* get/set visible */
/* get/set icon */
/* get/set visible cursor */
/* get/set vsync */
/* get focus */
/* get system window handle */

/* get/set cursor stuck */
/* get/set key repeat */

// void window_set_repeat(bool state);
//
// bool window_get_repeat(void);
//
// void window_set_keyboard_callback(window_keyboard_callback_f kcf);
//
// window_keyboard_callback_f window_get_keyboard_callback(void);

#endif
