#if 0
#include <glfw\glfw3.h>

struct input_t {
	_Bool down_keys[104];
	_Bool held_keys[104];
	_Bool released_keys[104];
};

static struct input_t input;

_Bool get_key(unsigned keycode);
_Bool get_key_down(unsigned keycode);
_Bool get_key_up(unsigned keycode);
#endif