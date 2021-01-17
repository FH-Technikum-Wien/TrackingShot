#ifndef TRACKINGSHOT_INPUT_H
#define TRACKINGSHOT_INPUT_H

#include <GLFW/glfw3.h>
#include <string>


class Input
{
public:
	static void ProcessContinuousInput(GLFWwindow* window);
	static void ProcessSingleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ProcessScrollInput(GLFWwindow* window, double xoffset, double yoffset);

	inline static std::string lastInput = "";
};

#endif //TRACKINGSHOT_INPUT_H