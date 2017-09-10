#if 0
#include "window.h"

window create_window(unsigned width, unsigned height, char* name)
{
	//window w = { width, height, name , 0 };
	window w = { width, height, name, glfwCreateWindow(width, height, name, 0, 0) };
	if (!w.gl_window)
	{
		glfwTerminate(); 
		puts("Window could not be created"); 
		w.gl_window = NULL; 
	}
	return w;
}
#endif