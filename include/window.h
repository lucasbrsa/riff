/* @TODO icons */
/* @TODO cursor handling */
/* @TODO macro getters/setters */
/* @TODO support X11 winapi wayland and cocoa */
/* @TODO FRAMEBUFFERS */

#ifndef _WINDOW_H
#define _WINDOW_H

#include <stdbool.h>

#include "error.h"
#include "os.h"

#if defined(OS_LINUX)
#	include "window_x11.h"
#else
#	error "unsupported platform"
#endif

#if defined(_WINDOW_X11_H)
typedef window_x11_t window_ihandle_t;
typedef window_x11_lib_t window_ilib_t;
#endif

/* temp */
#define WINDOW_KEY_LEN 256

/* temp */
#define WINDOW_MB_LEN 4

typedef struct {
	unsigned width, height;
	int redb, blueb, greenb;
	int refresh_rate;
} window_video_t;

#define WINDOW_STYLE_RESIZABLE  0x1
#define WINDOW_STYLE_VISIBLE    0x2
#define WINDOW_STYLE_DECORATED  0x4
#define WINDOW_STYLE_FOCUSED    0x8
#define WINDOW_STYLE_FLOATING   0x10
#define WINDOW_STYLE_MAXIMIZED  0x20

typedef struct window_s {
	bool opened;

	unsigned style;
	unsigned width, height, x, y;
	unsigned min_width, max_width;
	unsigned min_height, max_height;

	const char* title;

	char keys[WINDOW_KEY_LEN];
	char mbs[WINDOW_MB_LEN];

	float mx, my;

	window_s* next;
	window_video_t video;
	window_handle_t handle;
} window_t;

typedef struct {
	bool init;

	struct {
		const char* title;
		unsigned width, height, style;
	} hints;

	window_t* head;
	window_ilib_t internal;
} window_lib_t;

/* setup the window library */
void window_lib_init(void);

/* free the window lib */
void window_lib_free(void);

/* create a window, NULL on fail */
window_t* window_init(unsigned width, unsigned height, const char* title, unsigned style);

/* close a window */
void window_close(window_t* w);

/* is the window open? */
bool window_get_open(window_t* w);

/* get window position */
void window_get_pos(window_t*, unsigned* x, unsigned* y);

/* set the position of the window on screen */
status_t window_set_pos(window_t* w, unsigned x, unsigned y);

/* what is the current mode of this window */
void window_get_size(window_t* w, unsigned* width, unsigned* height);

/* change the mode of a window */
status_t window_set_size(window_t* w, unsigned width, unsigned height);

/* get title */
const char* window_get_title(window_t* w);

/* change the title of the window */
status_t window_set_title(window_t* w, const char* title);

/* get the current window limits */
status_t window_get_lims(window_t* w, unsigned* minw, unsigned* maxw, unsigned* minh, unsigned* maxh);

/* set window size limits */
void window_set_lims(window_t* w, unsigned minw, unsigned maxw, unsigned minh, unsigned maxh);

/* set visibility of window */
status_t window_set_visible(bool state);

/* use verticle sync */
status_t window_set_vsync(bool state);

/* set a limit of frame rate */
status_t window_set_flimit(float target);

/* set key repeat enabled */
status_t window_set_repeat(bool state);

/* get the window visiblity */
bool window_get_visible(void);

/* get the vsync state */
bool window_get_vsync(void);

/* get the current frame limit */
float window_get_flimit(void);

/* get the current frame rate (at that instant) */
float window_get_ftime(void);

/* get an average framerate */
float window_get_fps(void);

/* get the handle for that window */
window_handle_t window_get_handle(window_t* w);

/* is a mode valid, NULL for params to be ignored */
void window_validate_mode(unsigned width, unsigned height, unsigned bpp);

/* set the resize callback */
status_t window_set_onresize(window_t* w, void (resize)(int x, int y));

/* what is the video mode of the desktop */
void window_get_desktop_mode(unsigned* width, unsigned* height, unsigned* bpp);

/* what type of desktop is currently used i.e WINDOW_TYPE_X11 */
unsigned window_get_desktop_type(void);

/* TEMPORARY draw the window's contents on screen */
void window_flush(window_t* w);

#endif
