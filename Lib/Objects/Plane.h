#ifndef TRACKINGSHOT_PLANE_H
#define TRACKINGSHOT_PLANE_H

#include "Object.h"

class Plane : public Object
{
public:
	Plane(const Material& material, glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale = glm::vec3(1.0f)) : Object(material, position, eulerAngles) {
		glm::vec3 texScale = glm::vec3(scale);
		scale = scale / glm::vec3(2.0f);
		float* vertices = new float[18]{
			 scale.x,  scale.y, 0.0f,
			 scale.x, -scale.y, 0.0f,
			-scale.x,  scale.y, 0.0f,
			 scale.x, -scale.y, 0.0f,
			-scale.x, -scale.y, 0.0f,
			-scale.x,  scale.y, 0.0f,
		};

		float normals[18]{
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
		};

		float uvs[12]{
			texScale.x,       0.0f,
			texScale.x, texScale.y,
				  0.0f,       0.0f,
			texScale.x, texScale.y,
				  0.0f, texScale.y,
				  0.0f,       0.0f,
		};

		init(vertices, normals, uvs, 6);
	}
};

#endif //TRACKINGSHOT_PLANE_H
