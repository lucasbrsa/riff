#include "window_X11.h"
#include "vector.h"
#include "error.h"
#include "log.h"

/* the Xlib globals */
Display* g_dpy;
Window g_root;
XVisualInfo g_vi;
int g_screen;

typedef struct {
	/* the internal properties of a specific X11 window instance */
	Window win;
	Colormap cmap;

	bool opened;
	int width, height;
	int xpos, ypos;

	/* the global properties of the X11 lib */
} window_X11_t;

static window_X11_t l;

int window_X11_error_handler(Display* dpy, XErrorEvent* err) {
	static char txtbuf[128];

	XGetErrorText(dpy, err->error_code, txtbuf, sizeof(txtbuf));
	error_set("X: %s (%d)", txtbuf, err->error_code);

	return 0;
}

status_t window_X11_init(const char* title, unsigned width, unsigned height) {
	XSetErrorHandler(window_X11_error_handler);

	if (!(l.dpy = XOpenDisplay(NULL))) {
		error_set("could not open display");
		return STATUS_BAD;
	}

	l.root = DefaultRootWindow(l.dpy);
	l.screen = DefaultScreen(l.dpy);

	if (!XMatchVisualInfo(l.dpy, l.screen, 24, TrueColor, &l.vi)) {
		XCloseDisplay(l.dpy);
		error_set("could not match visual info on display");
		return STATUS_BAD;
	}

	unsigned long black = BlackPixel(l.dpy, l.screen);
	unsigned long white = WhitePixel(l.dpy, l.screen);

	l.cmap = XCreateColormap(l.dpy, l.root, l.vi.visual, AllocNone);

	XSetWindowAttributes wa;

	wa.border_pixel = black;
	wa.background_pixel = white;
	wa.override_redirect = false;
	wa.event_mask = ExposureMask;
	wa.colormap = l.cmap;

	l.win = XCreateWindow(l.dpy, l.root,
			0, 0, width, height, 0,
			l.vi.depth, InputOutput, l.vi.visual,
			CWBorderPixel | CWBackPixel | CWOverrideRedirect | CWEventMask | CWColormap,
			&wa);

	XStoreName(l.dpy, l.win, title);

	l.opened = true;
	l.gc = XCreateGC(l.dpy, l.win, 0, 0);

	XSetBackground(l.dpy, l.gc, white);
	XSetForeground(l.dpy, l.gc, black);

	XClearWindow(l.dpy, l.win);
	XMapRaised(l.dpy, l.win);

	XSelectInput(l.dpy, l.win, StructureNotifyMask);

	XEvent evnt;
	do {
		XNextEvent(l.dpy, &evnt);

		if (evnt.type == ConfigureNotify) {
			l.xpos = evnt.xconfigure.x;
			l.ypos = evnt.xconfigure.y;
			l.width = evnt.xconfigure.width;
			l.height = evnt.xconfigure.height;
		}
	} while (evnt.type != MapNotify);

	XSelectInput(l.dpy, l.win,
		StructureNotifyMask | VisibilityChangeMask);

	return STATUS_GOOD;
}

void window_X11_close(void) {
	if (l.opened) {
		XFreeColormap(l.dpy, l.cmap);
		XFreeGC(l.dpy, l.gc);
		XDestroyWindow(l.dpy, l.win);
		XCloseDisplay(l.dpy);

		l.opened = false;
	}
}

void window_X11_event_poll(void) {
	if (XPending(l.dpy)) {
		window_X11_event_wait();
	}
}

void window_X11_event_pause(float t) {
	clock_t st = clock() + (t * CLOCKS_PER_SEC);

	while (clock() < st) {
		if (XPending(l.dpy)) {
			window_X11_event_wait();
			return;
		}
	}
}

void window_X11_event_wait(void) {
	static XEvent e;

	XNextEvent(l.dpy, &e);

	switch (e.type) {
		case DestroyNotify:
			l.opened = false;
			break;
		case ConfigureNotify:
			l.xpos = e.xconfigure.x;
			l.ypos = e.xconfigure.y;
			l.width = e.xconfigure.width;
			l.height = e.xconfigure.height;
			break;
	}
}
