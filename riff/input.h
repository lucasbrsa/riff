#if 0

#include "generic.h"

#include <glfw\glfw3.h>

struct input_t {
	bool down_keys[104];
	bool held_keys[104];
	bool released_keys[104];
};

static struct input_t input;

bool get_key(unsigned keycode);
bool get_key_down(unsigned keycode);
bool get_key_up(unsigned keycode);
#endif
