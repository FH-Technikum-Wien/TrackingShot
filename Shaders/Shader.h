#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/matrix.hpp>

class Shader
{
public:
    // ID of the program
    unsigned int shaderProgramID;

    Shader(const char *vertexShaderPath, const char *fragmentShaderPath);

    ~Shader();

    void activate() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

    static std::string readFile(const char *filePath);
};

#endif //SHADER_H
