#ifndef TRACKINGSHOT_CUBE_H
#define TRACKINGSHOT_CUBE_H

#include "Object.h"
#include <algorithm>


class Cube : public Object {
public:
	Cube(const Material& material, glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale = glm::vec3(1.0f)) : Object(material, position, eulerAngles) {
		glm::vec3 texScale = glm::vec3(scale);
		scale = scale / glm::vec3(2.0f);

		float* vertices = new float[108]{
			// back face
			-scale.x, -scale.y, -scale.z,
			 scale.x, -scale.y, -scale.z,
			 scale.x,  scale.y, -scale.z,
			 scale.x,  scale.y, -scale.z,
			-scale.x,  scale.y, -scale.z,
			-scale.x, -scale.y, -scale.z,
			// front face
			-scale.x, -scale.y,  scale.z,
			 scale.x, -scale.y,  scale.z,
			 scale.x,  scale.y,  scale.z,
			 scale.x,  scale.y,  scale.z,
			-scale.x,  scale.y,  scale.z,
			-scale.x, -scale.y,  scale.z,
			// left face
			-scale.x,  scale.y,  scale.z,
			-scale.x,  scale.y, -scale.z,
			-scale.x, -scale.y, -scale.z,
			-scale.x, -scale.y, -scale.z,
			-scale.x, -scale.y,  scale.z,
			-scale.x,  scale.y,  scale.z,
			// right face
			 scale.x,  scale.y,  scale.z,
			 scale.x,  scale.y, -scale.z,
			 scale.x, -scale.y, -scale.z,
			 scale.x, -scale.y, -scale.z,
			 scale.x, -scale.y,  scale.z,
			 scale.x,  scale.y,  scale.z,
			 // bottom face
			-scale.x, -scale.y, -scale.z,
			 scale.x, -scale.y, -scale.z,
			 scale.x, -scale.y,  scale.z,
			 scale.x, -scale.y,  scale.z,
			-scale.x, -scale.y,  scale.z,
			-scale.x, -scale.y, -scale.z,
			// top face
			-scale.x,  scale.y, -scale.z,
			 scale.x,  scale.y, -scale.z,
			 scale.x,  scale.y,  scale.z,
			 scale.x,  scale.y,  scale.z,
			-scale.x,  scale.y,  scale.z,
			-scale.x,  scale.y, -scale.z,
		};

		float normals[108]{
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,

			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,

			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,

			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,

			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,

			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
		};

		float uvs[72]{
			0.0f, 0.0f,
			texScale.x, 0.0f,
			texScale.x, texScale.y,
			texScale.x, texScale.y,
			0.0f, texScale.y,
			0.0f, 0.0f,

			0.0f, 0.0f,
			texScale.x, 0.0f,
			texScale.x, texScale.y,
			texScale.x, texScale.y,
			0.0f, texScale.y,
			0.0f, 0.0f,

			 texScale.y, 0.0f,
			 texScale.y, texScale.z,
			 0.0f, texScale.z,
			 0.0f, texScale.z,
			 0.0f, 0.0f,
			 texScale.y, 0.0f,

			texScale.y, 0.0f,
			texScale.y, texScale.z,
			0.0f, texScale.z,
			0.0f, texScale.z,
			0.0f, 0.0f,
			texScale.y, 0.0f,

			0.0f, texScale.z,
			texScale.x, texScale.z,
			texScale.x, 0.0f,
			texScale.x, 0.0f,
			0.0f, 0.0f,
			0.0f, texScale.z,

			0.0f, texScale.z,
			texScale.x, texScale.z,
			texScale.x, 0.0f,
			texScale.x, 0.0f,
			0.0f, 0.0f,
			0.0f, texScale.z
		};

		init(vertices, normals, uvs, 36);
	}
};


#endif //TRACKINGSHOT_CUBE_H
