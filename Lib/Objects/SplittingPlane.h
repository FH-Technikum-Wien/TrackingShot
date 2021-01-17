#pragma once

#include "Object.h"

class SplittingPlane : public Object
{
public:
	SplittingPlane(glm::vec3 position, int axis, glm::vec3 max, glm::vec3 min) : Object(Material::WhiteMat(), glm::vec3(0), glm::vec3(0)) {
		// Translate to correct position on axis.
		glm::vec3 translation = glm::vec3(0);
		translation[axis] = position[axis];
		translate(translation);

		// Rotate so it splits correct axis.
		// Defined as z-axis plane -> No need to rotate for that case.
		

		// Draw as wireframe
		float* verticesX = new float[18]{
			0.0f, min[1], min[2],
			0.0f, max[1], min[2],
			0.0f, max[1], max[2],
			
			0.0f, max[1], max[2],
			0.0f, min[1], max[2],
			0.0f, min[1], min[2],
		};

		float* verticesY = new float[18]{
			min[0], 0.0f, min[2],
			min[0], 0.0f, max[2],
			max[0], 0.0f, max[2],
						   
			max[0], 0.0f, max[2],
			max[0], 0.0f, min[2],
			min[0], 0.0f, min[2],
		};

		float* verticesZ = new float[18]{
			min[0], min[1], 0.0f,
			max[0], min[1], 0.0f,
			max[0], max[1], 0.0f,

			max[0], max[1], 0.0f,
			min[0], max[1], 0.0f,
			min[0], min[1], 0.0f,
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
			   1, 0.0f,
			   1,    1,
			0.0f, 0.0f,
			   1,    1,
			0.0f,    1,
			0.0f, 0.0f,
		};

		unsigned int indices[6]{
			0,1,2,
			3,4,5
		};

		init(axis == 0 ? verticesX : axis == 1 ? verticesY : verticesZ, normals, uvs, 6, indices, 6);
	}
};
