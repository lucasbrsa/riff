#include "input.h"

_Bool get_key(unsigned keycode) { return input.held_keys[keycode]; }
_Bool get_key_down(unsigned keycode) { return input.down_keys[keycode]; }
_Bool get_key_up(unsigned keycode) { return input.released_keys[keycode]; }