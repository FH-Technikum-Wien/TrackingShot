#version 330 core
// Position with vertex attribute position 0
layout (location = 0) in vec3 aPos;
// Texture
layout (location = 1) in vec2 aTexCoord;

// Pass texture coordinates to frag
out vec2 TexCoord;

// Local to World
uniform mat4 modelMat;
// World to View (Camera)
uniform mat4 viewMat;
// View to Clip (perspective)
uniform mat4 projectionMat;

void main()
{
    gl_Position = projectionMat * viewMat * modelMat * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}