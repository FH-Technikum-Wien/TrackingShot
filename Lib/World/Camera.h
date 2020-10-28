#ifndef TRACKINGSHOT_CAMERA_H
#define TRACKINGSHOT_CAMERA_H

#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>


enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN

};

class Camera
{
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::quat Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	float MovementSpeed = 1.0f;
	float MouseSensitivity = 0.001f;

	bool IsWindowFocused = false;

	explicit Camera(glm::vec3 position);

	glm::mat4 GetViewMat();

	void ProcessMouse(double xPos, double yPos);

	void ProcessInput(CameraMovement movement);

private:
	glm::mat4 transform = glm::mat4(1.0f);
	glm::vec2 lastMousePos = glm::vec2(0.0f);

	float xOffset = 0.0f;
	float yOffset = 0.0f;
};
#endif //TRACKINGSHOT_CAMERA_H