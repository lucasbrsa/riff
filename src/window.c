#include "window.h"
#include "vector.h"
#include "error.h"
#include "log.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

static vector_t* window_stack;

typedef struct {
	Display* dis;
	Window win, root;

	int screen_id;
} window_X11_t;

int window_X11_error_handler(Display* dis, XErrorEvent err) {
	static char txtbuf[256];

	XGetErrorText(dis, err.error_code, txtbuf, sizeof(txtbuf));
	error_set("XErrorEvent %s (%d)", txtbuf, err.error_code);
}

int window_X11_create(const char* title, int w, int h) {
	if (window_stack == NULL)
		window_stack = vector_init(0, sizeof(window_X11_t*), free);

	window_X11_t* win = malloc(sizeof(window_X11_t));

	XSetErrorHandler(window_X11_error_handler);

	win->dis = XOpenDisplay(NULL);

	if (win->dis == NULL) {
		error_set("could not open X display");
		return -1;
	}

	win->screen_id = DefaultScreen(win->dis);

	int blk = BlackPixel(win->dis, win->screen_id);
	int wht = WhitePixel(win->dis, win->screen_id);

	win->win = XCreateSimpleWindow(win->dis,
			DefaultRootWindow(win->dis),
			0, 0, w, h, 0, blk, wht);

	XStoreName(win->dis, win->win, title);

	XMapRaised(win->dis, win->win);

	XSelectInput(win->dis, win->win, StructureNotifyMask);
	XEvent evnt;
	do {
		XNextEvent(win->dis, &evnt);
	} while (evnt.type != MapNotify);

	XSelectInput(win->dis, win->win, KeyPressMask | KeymapStateMask);

	int id = vector_size(window_stack);
	vector_push_back(window_stack, win);

	return id;
}

unsigned window_X11_poll_event(int win_id) {
	static XEvent evnt;
	window_X11_t* wptr = *(window_X11_t**)vector_at(window_stack, win_id);

	XNextEvent(wptr->dis, &evnt);

	if (evnt.type == KeymapNotify)
		XRefreshKeyboardMapping(&evnt.xmapping);

	if (evnt.type == KeyPress)
		return evnt.xkey.keycode;

	return 0;
}

void window_X11_close(int win_id) {
	if (window_stack == NULL)
		return;

	window_X11_t* wptr = *(window_X11_t**)vector_at(window_stack, win_id);

	XDestroyWindow(wptr->dis, wptr->win);
	XCloseDisplay(wptr->dis);

	wptr = NULL;

	vector_remove(window_stack, win_id);

	if (vector_size(window_stack) == 0) {
		vector_free(window_stack);
		window_stack = NULL;
	}
}

void window_X11_close_all(void) {
	/* not very efficient */
	int i = 0;
	while (window_stack && i < vector_size(window_stack))
		window_X11_close(i++);
}
