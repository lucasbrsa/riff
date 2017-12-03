#include "window.h"
#include "log.h"

#include <time.h>

int main() {
	int win = window_X11_create("demo_window", 200, 200);
	log_notice("created an X11 window");

	unsigned wait_time = time(0) + 5;
	while (time(0) < wait_time) {
		int keycode = window_X11_poll_event(win);
		if (keycode)
			log_info("key press: %d", keycode);
	}

	window_X11_close(win);
}
