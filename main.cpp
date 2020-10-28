#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <glm/matrix.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Lib/Consts.h"
#include "Lib/Util.h"
#include "Lib/World/Input.h"
#include "Lib/World/World.h"

#include "Lib/Objects/Cube.h"
#include "Lib/Objects/Plane.h"
#include "Lib/Objects/TriangleThing.h"

#include "Shaders/Shader.h"

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos);

void windowFocusCallback(GLFWwindow* window, int isFocused);

// FPS calculation
int numberOfFrames = 0;
double lastTime = 0.0;

int main() {
	//----------------------------------------------------------------------------------------------------
	// SETUP
	//----------------------------------------------------------------------------------------------------

	glfwInit();
	// Tell GLFW that we're using OpenGL version 3.3 .
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW to use Core_Profile -> Smaller subset without backwards-compatibility (not needed).
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Consts::SCREEN::WIDTH, Consts::SCREEN::HEIGHT, "", nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		throw std::runtime_error("Failed to create the GLFW-Window");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}

	// Set OpenGL viewport.
	glViewport(0, 0, Consts::SCREEN::WIDTH, Consts::SCREEN::HEIGHT);

	// Register events.
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mousePositionCallback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);
	glfwSetKeyCallback(window, Input::ProcessSingleInput);

	glEnable(GL_DEPTH_TEST);
	// Lock cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	// Show wireframe?
	if (Consts::RENDERING::USE_WIREFRAME_MODE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//----------------------------------------------------------------------------------------------------
	// RENDERING
	//----------------------------------------------------------------------------------------------------

	// Load textures
	unsigned int textureGrid = Util::LoadTexture(Consts::PATHS::TEXTURE_GRID, GL_RGB);
	unsigned int textureCrate = Util::LoadTexture(Consts::PATHS::TEXTURE_CRATE, GL_RGB);
	unsigned int textureTriangle = Util::LoadTexture(Consts::PATHS::TEXTURE_BRICK, GL_RGB);

	// Projection Matrix for adding perspective.
	glm::mat4 projectionMat;
	projectionMat = glm::perspective(glm::radians(45.0f), (float)Consts::SCREEN::WIDTH / (float)Consts::SCREEN::HEIGHT, 0.1f, 100.0f);

	// Add projection matrix to shader.
	Shader shader(Consts::PATHS::VERTEX_SHADER, Consts::PATHS::FRAGMENT_SHADER);
	shader.activate();
	shader.setMat4("projectionMat", projectionMat);

	World world;
	// Add objects.
	world.addObject(new Plane(&textureGrid, glm::vec3(0, -1, 0), glm::vec3(90, 0, 0)));
	world.addObject(new Cube(&textureCrate, glm::vec3(), glm::vec3()));
	world.addObject(new Cube(&textureCrate, glm::vec3(0.0f, 3.0f, -7.0f), glm::vec3()));
	world.addObject(new Cube(&textureCrate, glm::vec3(-3.0f, 0.0f, 2.0f), glm::vec3(45.0f, 45.0f, 0.0f)));
	world.addObject(new Cube(&textureCrate, glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(120.0f, 0.0f, 0.0f)));
	world.addObject(new Cube(&textureCrate, glm::vec3(2.0f, 1.0f, 4.0f), glm::vec3(70.0f, 120.0f, 45.0f)));
	world.addObject(new TriangleThing(&textureTriangle, glm::vec3(-3.0f, 0.0f, -4.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	world.addObject(new TriangleThing(&textureTriangle, glm::vec3(-4.0f, 2.0f, 4.0f), glm::vec3(90.0f, 0.0f, 45.0f)));
	world.addObject(new TriangleThing(&textureTriangle, glm::vec3(4.0f, 1.0f, -4.0f), glm::vec3(70.0f, 120.0f, 45.0f)));

	while (!glfwWindowShouldClose(window)) {

		// Sets one color for window (background).
		glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
		// Clear color buffer and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handle input.
		Input::ProcessContinuousInput(window);

		// Update world.
		world.update(shader);

		// Swaps the drawn buffer with the buffer that got written to.
		glfwSwapBuffers(window);
		// Checks if any events are triggered and executes callbacks.
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos) {
	World::GetCamera().ProcessMouse(xPos, yPos);
}

void windowFocusCallback(GLFWwindow* window, int isFocused) {
	World::GetCamera().IsWindowFocused = isFocused;
}