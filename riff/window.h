#include <stdio.h>
#include <glfw\glfw3.h>

typedef struct window_t {
	unsigned width;
	unsigned height;
	const char* name;
	GLFWwindow* gl_window;
} window;

window create_window(unsigned width, unsigned height, char* name);
