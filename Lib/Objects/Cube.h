#ifndef TRACKINGSHOT_CUBE_H
#define TRACKINGSHOT_CUBE_H

#include "Object.h"


class Cube : public Object {
public:
	Cube(const Material& material, glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale = glm::vec3(1.0f)) : Object(material, position, eulerAngles) {
		glm::vec3 texScale = glm::vec3(scale);
		scale = scale / glm::vec3(2.0f);

		float vertices[288]{
			-scale.x, -scale.y, -scale.z, 0.0f,  0.0f, -texScale.x, 0.0f, 0.0f,
			 scale.x, -scale.y, -scale.z, 0.0f,  0.0f, -texScale.x, texScale.y, 0.0f,
			 scale.x,  scale.y, -scale.z, 0.0f,  0.0f, -texScale.x, texScale.y, texScale.z,
			 scale.x,  scale.y, -scale.z, 0.0f,  0.0f, -texScale.x, texScale.y, texScale.z,
			-scale.x,  scale.y, -scale.z, 0.0f,  0.0f, -texScale.x, 0.0f, texScale.z,
			-scale.x, -scale.y, -scale.z, 0.0f,  0.0f, -texScale.x, 0.0f, 0.0f,

			-scale.x, -scale.y,  scale.z, 0.0f,  0.0f,  texScale.x, 0.0f, 0.0f,
			 scale.x, -scale.y,  scale.z, 0.0f,  0.0f,  texScale.x, texScale.y, 0.0f,
			 scale.x,  scale.y,  scale.z, 0.0f,  0.0f,  texScale.x, texScale.y, texScale.z,
			 scale.x,  scale.y,  scale.z, 0.0f,  0.0f,  texScale.x, texScale.y, texScale.z,
			-scale.x,  scale.y,  scale.z, 0.0f,  0.0f,  texScale.x, 0.0f, texScale.z,
			-scale.x, -scale.y,  scale.z, 0.0f,  0.0f,  texScale.x, 0.0f, 0.0f,

			-scale.x,  scale.y,  scale.z, 1.0f,  0.0f,  0.0f, texScale.y, 0.0f,
			-scale.x,  scale.y, -scale.z, 1.0f,  0.0f,  0.0f, texScale.y, texScale.z,
			-scale.x, -scale.y, -scale.z, 1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			-scale.x, -scale.y, -scale.z, 1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			-scale.x, -scale.y,  scale.z, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-scale.x,  scale.y,  scale.z, 1.0f,  0.0f,  0.0f, texScale.y, 0.0f,

			 scale.x,  scale.y,  scale.z, 1.0f,  0.0f,  0.0f, texScale.y, 0.0f,
			 scale.x,  scale.y, -scale.z, 1.0f,  0.0f,  0.0f, texScale.y, texScale.z,
			 scale.x, -scale.y, -scale.z, 1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			 scale.x, -scale.y, -scale.z, 1.0f,  0.0f,  0.0f, 0.0f, texScale.z,
			 scale.x, -scale.y,  scale.z, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 scale.x,  scale.y,  scale.z, 1.0f,  0.0f,  0.0f, texScale.y, 0.0f,

			-scale.x, -scale.y, -scale.z, 0.0f, -1.0f,  0.0f, 0.0f, texScale.z,
			 scale.x, -scale.y, -scale.z, 0.0f, -1.0f,  0.0f, texScale.y, texScale.z,
			 scale.x, -scale.y,  scale.z, 0.0f, -1.0f,  0.0f, texScale.y, 0.0f,
			 scale.x, -scale.y,  scale.z, 0.0f, -1.0f,  0.0f, texScale.y, 0.0f,
			-scale.x, -scale.y,  scale.z, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-scale.x, -scale.y, -scale.z, 0.0f, -1.0f,  0.0f, 0.0f, texScale.z,

			-scale.x,  scale.y, -scale.z, 0.0f,  1.0f,  0.0f, 0.0f, texScale.z,
			 scale.x,  scale.y, -scale.z, 0.0f,  1.0f,  0.0f, texScale.y, texScale.z,
			 scale.x,  scale.y,  scale.z, 0.0f,  1.0f,  0.0f, texScale.y, 0.0f,
			 scale.x,  scale.y,  scale.z, 0.0f,  1.0f,  0.0f, texScale.y, 0.0f,
			-scale.x,  scale.y,  scale.z, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-scale.x,  scale.y, -scale.z, 0.0f,  1.0f,  0.0f, 0.0f, texScale.z
		};
		init(vertices, 288);
	}
};


#endif //TRACKINGSHOT_CUBE_H
