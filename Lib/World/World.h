#pragma once
#include "Camera.h"
#include "../Objects/Object.h"
#include "../../Shaders/Shader.h"
#include "../Datatypes/Path.h"


enum class MovementSpeedLevel
{
	SLOW,
	NORMAL,
	FAST,
	SONIC
};

class World
{
public:
	inline static float DeltaTime = 0.0f;
	inline static bool FreeFly = true;

	static void SetMovementSpeed(MovementSpeedLevel level);
	static Camera& GetCamera();
	static void ToggleFreeFly();

	static void AddPathPoint();
	static void RemoveLastPathPoint();
	static void ResetPath();
	static void GoToNextPathPoint();
	static void GoToPreviousPathPoint();
	static void GoToCurrentPathPoint();
	static void GoToFirstPathPoint();

	static float GetBumpiness();
	static void SetBumpiness(float newBumpiness);

	void addObject(Object* object);
	void update(Shader& shader);
	void renderWorld(Shader& shader);

private:
	inline static Camera camera{ glm::vec3(0.0f, 0.0f, 3.0f) };
	inline static Path path = Path::DefaultPath();
	inline static bool isUsingDefaultPath = true;

	std::vector<Object*> objects;
	float lastFrameTime = 0.0f;

	/// <summary>
	/// 0   -> Nothing
	/// 1	-> Default
	/// > 1 -> Increase
	/// < 1 -> Decrease
	/// </summary>
	inline static float normalMapBumpiness = 1;
};