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
#include "Lib/Objects/SplittingPlane.h"
#include "Lib/Objects/TriangleObject.h"

#include "Shaders/Shader.h"
#include "Lib/World/Light.h"

#include "Util/objLoader/OBJ_Loader.h"
#include "Intersection/KdTree.h"

#include <chrono>

using namespace std;

void setup();

void addObjects(World& world);

void addComplexObject(World& world);

void visualizeRaycast(KdStructs::RayHit* hit, Camera camera, glm::vec3 directionVector);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos);

void windowFocusCallback(GLFWwindow* window, int isFocused);

// FPS calculation
int numberOfFrames = 0;
double lastTime = 0.0;

Shader shader;
Shader depthMapShader;
World world;
Light light;

glm::mat4 lightSpaceMat;
unsigned int depthMapFBO;
unsigned int depthMap;


GLFWwindow* window = nullptr;
bool useAntiAliasing = false;
int samplingMode = 1;

double delay = 0.5;
double timePressed = 0;

// Kd-Tree
KdTree* kdtree = nullptr;

int main()
{
	setup();

	timePressed = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		// Sets one color for window (background).
		glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
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
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Reset viewport.
		glViewport(0, 0, Consts::SCREEN::WIDTH, Consts::SCREEN::HEIGHT);
		// Clear color buffer and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.activate();
		shader.setVec3("lightPos", light.position);
		shader.setMat4("lightSpaceMat", lightSpaceMat);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		// Render world.
		world.update(shader);

		// Swaps the drawn buffer with the buffer that got written to.
		glfwSwapBuffers(window);
		// Checks if any events are triggered and executes callbacks.
		glfwPollEvents();


		// Show last input
		std::string lastInput = "Last Input: " + Input::lastInput;
		glfwSetWindowTitle(window, lastInput.c_str());

		// Antialiasing input
		if (useAntiAliasing)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
		// Change anti aliasing mode. Requires restart.
		if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
		{
			samplingMode = (samplingMode + 1) % 8;
			glfwDestroyWindow(window);
			setup();
		}

		// Change anti aliasing mode. Requires restart.
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && timePressed + delay < glfwGetTime())
		{
			timePressed = glfwGetTime();
			// Get ray origin and direction
			Camera camera = World::GetCamera();
			KdStructs::Vector position = KdStructs::Vector(camera.Position.x, camera.Position.y, camera.Position.z);
			glm::vec3 directionVector = camera.Orientation * glm::vec3(0, 0, -1);
			// Normalize
			directionVector = glm::normalize(directionVector);
			KdStructs::Vector direction = KdStructs::Vector(directionVector.x, directionVector.y, directionVector.z);

			// Cast ray into scene
			KdStructs::RayHit* hit = nullptr;
			auto start = std::chrono::high_resolution_clock::now();
			kdtree->raycast(KdStructs::Ray(position, direction, 1000), hit);
			auto end = std::chrono::high_resolution_clock::now();
			std::cout << "Raycast time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds." << std::endl;

			visualizeRaycast(hit, camera, directionVector);
			
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS ) {
			world.intersectionPoint = nullptr;
			world.intersectionTriangle = nullptr;
			world.rayLine = nullptr;
		}

	}

	glfwTerminate();
	return 0;
}

void setup()
{
	//----------------------------------------------------------------------------------------------------
	// SETUP
	//----------------------------------------------------------------------------------------------------

	glfwInit();
	// Tell GLFW that we're using OpenGL version 3.3 .
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW to use Core_Profile -> Smaller subset without backwards-compatibility (not needed).
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Set Anti-Aliasing
	glfwWindowHint(GLFW_SAMPLES, samplingMode);


	window = glfwCreateWindow(Consts::SCREEN::WIDTH, Consts::SCREEN::HEIGHT, "", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create the GLFW-Window");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
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

	shader = Shader(Consts::PATHS::SHADOW_VERT_SHADER, Consts::PATHS::SHADOW_FRAG_SHADER);
	shader.activate();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);
	shader.setInt("normalMap", 2);
	shader.setMat4("projectionMat", projectionMat);
	shader.setFloat("ambientLightAmount", 0.5f);


	light = Light(glm::vec3(-11.0f, 10.0f, 0.0f), 3.0f);
	lightSpaceMat = light.activateLight(shader);

	addObjects(world);
	//addComplexObject(world);


	// KD-TREE
	std::vector<float> vertices = world.getAllObjectVertices();
	std::vector<unsigned int> indices = world.getAllObjectIndices();

	auto start = std::chrono::high_resolution_clock::now();
	kdtree = new KdTree(&vertices[0], vertices.size() / 3);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Building time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds." << std::endl;
	kdtree->printStatistics();

	// Add bounding boxes.
	std::vector<KdStructs::Node*> nodes = kdtree->getNodes();
	for (KdStructs::Node* node : nodes) {
		glm::vec3 position = glm::vec3(node->point->pos[0], node->point->pos[1], node->point->pos[2]);
		glm::vec3 max = glm::vec3(node->max[0], node->max[1], node->max[2]);
		glm::vec3 min = glm::vec3(node->min[0], node->min[1], node->min[2]);

		world.addBoundingBox(new BoundingBox(position, max, min));
	}

	//----------------------------------------------------------------------------------------------------
	// SHADOWS
	//----------------------------------------------------------------------------------------------------

	// DepthMap Shader
	depthMapShader = Shader(Consts::PATHS::DEPTH_MAP_VERT_SHADER, Consts::PATHS::DEPTH_MAP_FRAG_SHADER);
	depthMapShader.activate();
	// Framebuffer for rendering the depthMap.
	glGenFramebuffers(1, &depthMapFBO);
	// Create depthMap texture.
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
}

void addObjects(World& world)
{

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
	world.addObject(new TriangleThing(rocksMat, glm::vec3(4.0f, 1.0f, -4.0f), glm::vec3(0.0f, 120.0f, 45.0f)));
}

void addComplexObject(World& world) {
	objl::Loader loader;
	loader.LoadFile("Resources/Nubian/nubian_complex.obj");
	objl::Mesh mesh = loader.LoadedMeshes[0];
	std::vector<float>* vertices = new std::vector<float>();
	std::vector<float>* normals = new std::vector<float>();
	std::vector<float>* uvs = new std::vector<float>();
	for (const objl::Vertex& vertex : mesh.Vertices)
	{
		vertices->push_back(vertex.Position.X);
		vertices->push_back(vertex.Position.Y);
		vertices->push_back(vertex.Position.Z);

		normals->push_back(vertex.Normal.X);
		normals->push_back(vertex.Normal.Y);
		normals->push_back(vertex.Normal.Z);

		uvs->push_back(vertex.TextureCoordinate.X);
		uvs->push_back(vertex.TextureCoordinate.Y);
	}

	std::vector<unsigned int>* indices = new std::vector<unsigned int>(mesh.Indices);
	//unsigned int texture = Util::LoadTexture("Resources/Plant/texture.jpg", GL_RGB);
	//unsigned int normal = Util::LoadTexture("Resources/Plant/normals.jpg", GL_RGB);
	//Material mat = Material(&texture, &normal, glm::vec3(1));
	Material mat = Material::WoodMat();

	Object* object = new Object(mat, glm::vec3(0), glm::vec3(0));
	object->init(vertices->data(), normals->data(), uvs->data(), mesh.Vertices.size(), indices->data(), indices->size());
	world.addObject(object);
}

void visualizeRaycast(KdStructs::RayHit* hit, Camera camera, glm::vec3 directionVector)
{
	// Add ray to scene.
	world.addRay(new Line(camera.Position, camera.Position + directionVector * 1000.0f));
	if (hit != nullptr) {
		std::cout << "Hit at: " << hit->position << std::endl;
		// Add colorful triangle to scene.
		glm::vec3 hitPosition = glm::vec3(hit->position[0], hit->position[1], hit->position[2]);
		float* vertices = new float[9]{
			hit->triangle->a[0], hit->triangle->a[1], hit->triangle->a[2],
			hit->triangle->b[0], hit->triangle->b[1], hit->triangle->b[2],
			hit->triangle->c[0], hit->triangle->c[1], hit->triangle->c[2],
		};
		TriangleObject* triangle = new TriangleObject(vertices, glm::vec3(1, 0, 0));
		// Move triangle a bit in camera direction to prevent overlapping.
		triangle->translate(directionVector * -0.001f);
		// Add ray to intersection.
		world.addRay(new Line(camera.Position, hitPosition));
		// Add intersecion point.
		world.addIntersection(new Point(hitPosition), triangle);
	}
	else {
		std::cout << "No hit!" << std::endl;
		// Remove intersection point and triangle.
		world.intersectionPoint = nullptr;
		world.intersectionTriangle = nullptr;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	World::GetCamera().ProcessMouse(xPos, yPos);
}

void windowFocusCallback(GLFWwindow* window, int isFocused)
{
	World::GetCamera().IsWindowFocused = isFocused;
}