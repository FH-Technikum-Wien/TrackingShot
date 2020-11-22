#ifndef TRACKINGSHOT_CUBE_H
#define TRACKINGSHOT_CUBE_H

#include "Object.h"
#include <algorithm>


class Cube : public Object {
public:
	Cube(const Material& material, glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale = glm::vec3(1.0f)) : Object(material, position, eulerAngles) {
		glm::vec3 texScale = glm::vec3(scale);
		scale = scale / glm::vec3(2.0f);

		float vertices[288]{
			// back face
			-scale.x, -scale.y, -scale.z, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 scale.x, -scale.y, -scale.z, 0.0f,  0.0f, -1.0f, texScale.x, 0.0f,
			 scale.x,  scale.y, -scale.z, 0.0f,  0.0f, -1.0f, texScale.x, texScale.y,
			 scale.x,  scale.y, -scale.z, 0.0f,  0.0f, -1.0f, texScale.x, texScale.y,
			-scale.x,  scale.y, -scale.z, 0.0f,  0.0f, -1.0f, 0.0f, texScale.y,
			-scale.x, -scale.y, -scale.z, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			// front face
			-scale.x, -scale.y,  scale.z, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			 scale.x, -scale.y,  scale.z, 0.0f,  0.0f,  1.0f, texScale.x, 0.0f,
			 scale.x,  scale.y,  scale.z, 0.0f,  0.0f,  1.0f, texScale.x, texScale.y,
			 scale.x,  scale.y,  scale.z, 0.0f,  0.0f,  1.0f, texScale.x, texScale.y,
			-scale.x,  scale.y,  scale.z, 0.0f,  0.0f,  1.0f, 0.0f, texScale.y,
			-scale.x, -scale.y,  scale.z, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			// left face
			-scale.x,  scale.y,  scale.z, -1.0f,  0.0f,  0.0f, texScale.y, 0.0f,
			-scale.x,  scale.y, -scale.z, -1.0f,  0.0f,  0.0f, texScale.y, texScale.z,
			-scale.x, -scale.y, -scale.z, -1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			-scale.x, -scale.y, -scale.z, -1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			-scale.x, -scale.y,  scale.z, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-scale.x,  scale.y,  scale.z, -1.0f,  0.0f,  0.0f, texScale.y, 0.0f,
			// right face
			 scale.x,  scale.y,  scale.z, 1.0f,  0.0f,  0.0f, texScale.y, 0.0f,
			 scale.x,  scale.y, -scale.z, 1.0f,  0.0f,  0.0f, texScale.y, texScale.z,
			 scale.x, -scale.y, -scale.z, 1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			 scale.x, -scale.y, -scale.z, 1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			 scale.x, -scale.y,  scale.z, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 scale.x,  scale.y,  scale.z, 1.0f,  0.0f,  0.0f, texScale.y, 0.0f,
			 // bottom face
			-scale.x, -scale.y, -scale.z, 0.0f, -1.0f,  0.0f, 0.0f, texScale.z,
			 scale.x, -scale.y, -scale.z, 0.0f, -1.0f,  0.0f, texScale.x, texScale.z,
			 scale.x, -scale.y,  scale.z, 0.0f, -1.0f,  0.0f, texScale.x, 0.0f,
			 scale.x, -scale.y,  scale.z, 0.0f, -1.0f,  0.0f, texScale.x, 0.0f,
			-scale.x, -scale.y,  scale.z, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-scale.x, -scale.y, -scale.z, 0.0f, -1.0f,  0.0f, 0.0f, texScale.z,
			// top face
			-scale.x,  scale.y, -scale.z, 0.0f,  1.0f,  0.0f, 0.0f, texScale.z,
			 scale.x,  scale.y, -scale.z, 0.0f,  1.0f,  0.0f, texScale.x, texScale.z,
			 scale.x,  scale.y,  scale.z, 0.0f,  1.0f,  0.0f, texScale.x, 0.0f,
			 scale.x,  scale.y,  scale.z, 0.0f,  1.0f,  0.0f, texScale.x, 0.0f,
			-scale.x,  scale.y,  scale.z, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-scale.x,  scale.y, -scale.z, 0.0f,  1.0f,  0.0f, 0.0f, texScale.z
		};
		init(vertices, 288);
	}
};


#endif //TRACKINGSHOT_CUBE_H
