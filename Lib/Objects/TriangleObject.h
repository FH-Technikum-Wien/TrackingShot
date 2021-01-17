#pragma once

#include "Object.h"

class TriangleObject : public Object
{
public:
	TriangleObject(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale = glm::vec3(1.0f)) : Object(Material(glm::vec3(0,0,0.8)), position, eulerAngles) {
		glm::vec3 texScale = glm::vec3(scale);
		scale = scale / glm::vec3(2.0f);
		float* vertices = new float[9]{
			-scale.x, -scale.y, 0.0f,
			 scale.x, -scale.y, 0.0f,
			 scale.x,  scale.y, 0.0f,
		};

		float* normals = new float[9]{
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
		};

		float* uvs = new float[6]{
			texScale.x,       0.0f,
			texScale.x, texScale.y,
				  0.0f,       0.0f,
		};

		unsigned int* indices = new unsigned int[3]{
			0,1,2
		};

		init(vertices, normals, uvs, 3, indices, 3);
	}

	TriangleObject(float* vertices, glm::vec3 color) : Object(Material(color), glm::vec3(0), glm::vec3(0)) {
		float* normals = new float[9]{
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
		};

		float* uvs = new float[6]{
			   1, 0.0f,
			   1,    1,
			0.0f, 0.0f,
		};

		unsigned int* indices = new unsigned int[3]{
			0,1,2
		};

		init(vertices, normals, uvs, 3, indices, 3);
	}
};
