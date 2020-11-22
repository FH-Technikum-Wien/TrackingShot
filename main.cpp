#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Lib/Consts.h"
#include "Lib/Util.h"
#include "Lib/World/Input.h"
#include "Lib/World/World.h"

#include "Lib/Objects/Cube.h"
#include "Lib/Objects/Plane.h"
#include "Lib/Objects/TriangleThing.h"

#include "Shaders/Shader.h"
#include "Lib/World/Light.h"

using namespace std;

void addObjects(World& world);

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
	glfwSetScrollCallback(window, Input::ProcessScrollInput);

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


	// Projection Matrix for adding perspective.
	glm::mat4 projectionMat;
	projectionMat = glm::perspective(glm::radians(45.0f), (float)Consts::SCREEN::WIDTH / (float)Consts::SCREEN::HEIGHT, 0.1f, 100.0f);

	Shader shader(Consts::PATHS::SHADOW_VERT_SHADER, Consts::PATHS::SHADOW_FRAG_SHADER);
	shader.activate();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);
	shader.setInt("normalMap", 2);
	shader.setMat4("projectionMat", projectionMat);
	shader.setFloat("ambientLightAmount", 0.5f);


	Light light = Light(glm::vec3(-6.0f, 8.0f, -1.0f), 2.0f);
	glm::mat4 lightSpaceMat = light.activateLight(shader);

	World world;
	addObjects(world);


	//----------------------------------------------------------------------------------------------------
	// SHADOWS
	//----------------------------------------------------------------------------------------------------

	// DepthMap Shader
	Shader depthMapShader(Consts::PATHS::DEPTH_MAP_VERT_SHADER, Consts::PATHS::DEPTH_MAP_FRAG_SHADER);
	depthMapShader.activate();
	// Framebuffer for rendering the depthMap.
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// Create depthMap texture.
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// Set resolution and only use 'DEPTH_COMPONENT', only need depth.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Consts::RENDERING::SHADOW_WITH, Consts::RENDERING::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Everything outside of light frustum has depth of 1.0 -> no shadow.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Attach depth texture to FrameBufferObject's depth buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// Tell OpenGL that we don't need any color.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		// Sets one color for window (background).
		glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
		// Clear color buffer and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handle input.
		Input::ProcessContinuousInput(window);

		// Render depth of scene to depthMap texture
		depthMapShader.activate();
		depthMapShader.setMat4("lightSpaceMat", lightSpaceMat);
		glViewport(0, 0, Consts::RENDERING::SHADOW_WITH, Consts::RENDERING::SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Prevent peter panning by using back-faces
		glCullFace(GL_FRONT);
		// Render world's depth
		world.renderWorld(depthMapShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);

		// Reset viewport.
		glViewport(0, 0, Consts::SCREEN::WIDTH, Consts::SCREEN::HEIGHT);
		// Clear color buffer and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.activate();
		shader.setVec3("lightPos", light.position);
		shader.setMat4("lightSpaceMat", lightSpaceMat);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		// Render world with shadows.
		world.update(shader);

		// Swaps the drawn buffer with the buffer that got written to.
		glfwSwapBuffers(window);
		// Checks if any events are triggered and executes callbacks.
		glfwPollEvents();


		// Show bumpiness
		std::string bumpiness = std::to_string(World::GetBumpiness());
		glfwSetWindowTitle(window, bumpiness.c_str());
	}

	glfwTerminate();
	return 0;
}

void addObjects(World& world) {

	Material rocksMat = Material::RocksMat();
	Material woodMat = Material::WoodMat();
	Material brickMat = Material::BrickMat();
	Material brick2Mat = Material::Brick2Mat();

	// Add objects.
	world.addObject(new Cube(woodMat, glm::vec3(0, -2, 0), glm::vec3(0, 0, 0), glm::vec3(20.0f, 2.0f, 20.0f)));
	world.addObject(new Cube(rocksMat, glm::vec3(), glm::vec3()));
	world.addObject(new Cube(brick2Mat, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(), glm::vec3(0.5f, 1.0f, 0.5f)));
	world.addObject(new Cube(rocksMat, glm::vec3(0.0f, 3.0f, -7.0f), glm::vec3()));
	world.addObject(new Cube(brickMat, glm::vec3(-3.0f, 0.0f, 2.0f), glm::vec3(45.0f, 45.0f, 0.0f)));
	world.addObject(new Cube(brick2Mat, glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(120.0f, 0.0f, 0.0f)));
	world.addObject(new Cube(brick2Mat, glm::vec3(2.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.25f, 2.0f)));
	world.addObject(new Cube(brickMat, glm::vec3(2.0f, 1.0f, 4.0f), glm::vec3(70.0f, 120.0f, 45.0f)));

	world.addObject(new TriangleThing(brickMat, glm::vec3(-3.0f, 0.0f, -4.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	world.addObject(new TriangleThing(woodMat, glm::vec3(-4.0f, 2.0f, 4.0f), glm::vec3(90.0f, 0.0f, 45.0f)));
	world.addObject(new TriangleThing(rocksMat, glm::vec3(4.0f, 1.0f, -4.0f), glm::vec3(70.0f, 120.0f, 45.0f)));
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