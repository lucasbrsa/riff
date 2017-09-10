#include "context.h"

context create_context(unsigned width, unsigned height, char* name) {

	/* initialise glfw */
	if (!glfwInit())
		puts("Could not initialise GLFW!");
	else
		puts("GLFW initialised.");

	context c = { create_window(width, height, name) };

	/* setting window assumes that */
	glfwMakeContextCurrent(c.window.gl_window);

	/* initialise glew */
	if (glewInit() != GLEW_OK)
		puts("Could not initialise GLEW!");
	else
		puts("GLEW initialised.");
	
	return c;
}