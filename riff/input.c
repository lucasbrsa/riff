#if 0
#include "input.h"

bool get_key(unsigned keycode) { return input.held_keys[keycode]; }
bool get_key_down(unsigned keycode) { return input.down_keys[keycode]; }
bool get_key_up(unsigned keycode) { return input.released_keys[keycode]; }
#endif
