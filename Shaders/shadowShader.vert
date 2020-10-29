#version 330 core
// Position with vertex attribute position 0
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture
layout (location = 2) in vec2 aTexCoord;


out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

// Local to World
uniform mat4 modelMat;
// World to View (Camera)
uniform mat4 viewMat;
// View to Clip (perspective)
uniform mat4 projectionMat;
// World Space to light space.
uniform mat4 lightSpaceMat;

void main()
{
    vec3 fragPos = vec3(modelMat * vec4(aPos, 1.0));

    vs_out.FragPos = fragPos;
    vs_out.Normal = transpose(inverse(mat3(modelMat))) * aNormal;
    vs_out.TexCoords = aTexCoord;
    vs_out.FragPosLightSpace = lightSpaceMat * vec4(fragPos, 1.0);

    gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0);
}