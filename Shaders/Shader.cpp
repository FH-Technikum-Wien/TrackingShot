#include "Shader.h"
#include <glad\glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm\gtc\type_ptr.hpp>

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    std::string vertexShaderString = readFile(vertexShaderPath);
    std::string fragmentShaderString = readFile(fragmentShaderPath);

    const char *vertexShaderCode = vertexShaderString.c_str();
    const char *fragmentShaderCode = fragmentShaderString.c_str();

    int success;
    char infoLog[512];

    // Create shader Objects
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgramID = glCreateProgram();

    // Attach shader source to shader object
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    // Compile shader (at run-time)
    glCompileShader(vertexShader);
    // Check compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Attach shader source to shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    // Compile shader (at run-time)
    glCompileShader(fragmentShader);
    // Check compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Shader program
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);
    // Check compilation
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_ERROR\n" << infoLog << std::endl;
    }

    // Delete Shaders after linking, not needed anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteShader(shaderProgramID);
}

std::string Shader::readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        return "";
    }

    std::stringstream fileStringStream;
    // Read file's buffer into stream
    fileStringStream << fileStream.rdbuf();
    // Close file
    fileStream.close();

    return fileStringStream.str();
}

void Shader::activate() const
{
    glUseProgram(shaderProgramID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
