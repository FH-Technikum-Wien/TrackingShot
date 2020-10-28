#ifndef TRACKINGSHOT_TRIANGLETHING_H
#define TRACKINGSHOT_TRIANGLETHING_H


#include "Object.h"

class TriangleThing : public Object {
public:
	TriangleThing(const unsigned int* texture, glm::vec3 position, glm::vec3 eulerAngles) : Object(texture, position,
		eulerAngles) {
		init(vertices, 60);
	}

private:
	const float val = (float)sqrt(3) / 2;

	const float vertices[60]{
		//front
		1.0f, val, 0.0f, 1.0f, 0.0f,
		-1.0f, val, 0.0f, 0.0f, 0.0f,
		0.0f, -val, 1.0f, 0.5f, 1.0f,

		//left
		-1.0f, val, 0.0f, 0.5f, 1.0f,
		0.0f, -val, -1.0f, 0.0f, 0.0f,
		0.0f, -val, 1.0f, 1.0f, 0.0f,

		//right
		1.0f, val, 0.0f, 0.5f, 1.0f,
		0.0f, -val, 1.0f, 0.0f, 0.0f,
		0.0f, -val, -1.0f, 1.0f, 0, .0f

		//back
		- 1.0f, val, 0.0f, 1.0f, 0.0f,
		1.0f, val, 0.0f, 0.0f, 0.0f,
		0.0f, -val, -1.0f, 0.5f, 1.0f
	};
};


#endif //TRACKINGSHOT_TRIANGLETHING_H
