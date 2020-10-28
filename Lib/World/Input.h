#ifndef TRACKINGSHOT_INPUT_H
#define TRACKINGSHOT_INPUT_H

#include <GLFW/glfw3.h>


class Input
{
public:
	static void ProcessContinuousInput(GLFWwindow* window);
	static void ProcessSingleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif //TRACKINGSHOT_INPUT_H