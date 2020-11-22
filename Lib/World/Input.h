#ifndef TRACKINGSHOT_INPUT_H
#define TRACKINGSHOT_INPUT_H

#include <GLFW/glfw3.h>


class Input
{
public:
	static void ProcessContinuousInput(GLFWwindow* window);
	static void ProcessSingleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ProcessScrollInput(GLFWwindow* window, double xoffset, double yoffset);
};

#endif //TRACKINGSHOT_INPUT_H