#ifndef TRACKINGSHOT_TRIANGLETHING_H
#define TRACKINGSHOT_TRIANGLETHING_H


#include "Object.h"

class TriangleThing : public Object {
public:
	TriangleThing(const Material& material, glm::vec3 position, glm::vec3 eulerAngles) : Object(material, position,
		eulerAngles) {
		init(vertices, normals, uvs, 12);
	}

private:
	float val = (float)sqrt(3) / 2;

	float vertices[36]{
		//front
		 1.0f,  val,  0.0f,
		 0.0f, -val,  1.0f,
		-1.0f,  val,  0.0f,

		//left
		-1.0f,  val,  0.0f,
		 0.0f, -val,  1.0f,
		 0.0f, -val, -1.0f,

		//right
		 1.0f,  val,  0.0f,
		 0.0f, -val, -1.0f,
		 0.0f, -val,  1.0f,

		//back
		-1.0f,  val,  0.0f,
		 0.0f, -val, -1.0f,
		 1.0f,  val,  0.0f,
	};

	float normals[36]{
		0.0f, val,  1.0f,
		0.0f, val,  1.0f,
		0.0f, val,  1.0f,


		1.0f, -val, 0.0f,
		1.0f, -val, 0.0f,
		1.0f, -val, 0.0f,


		1.0, -val, 0.0f,
		1.0, -val, 0.0f,
		1.0, -val, 0.0f,


		0.0f, val, -1.0f,
		0.0f, val, -1.0f,
		0.0f, val, -1.0f,
	};

	float uvs[24]{
		 1.0f, 0.0f,
		 0.5f, 1.0f,
		 0.0f, 0.0f,


		 0.5f, 1.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f,


		0.5f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,


		 1.0f, 0.0f,
		 0.5f, 1.0f,
		 0.0f, 0.0f,
	};

};


#endif //TRACKINGSHOT_TRIANGLETHING_H
