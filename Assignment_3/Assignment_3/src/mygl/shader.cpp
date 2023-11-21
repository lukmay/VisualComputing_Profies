#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace detail
{
    void compile(GLuint handle, const char* source, const int size)
    {
        GLint compileResult = 0;

        glShaderSource(handle, 1, &source, &size);
        glCompileShader(handle);
        glGetShaderiv(handle, GL_COMPILE_STATUS, &compileResult);

        if(compileResult == GL_FALSE)
        {
            GLint infoLength = 0;
            std::string shaderLog;

            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLength);
            shaderLog.resize(static_cast<std::size_t>(infoLength));
            glGetShaderInfoLog(handle, infoLength, nullptr, &shaderLog[0]);

            std::cerr << "[Shader] ERROR compiling shader:\n" << shaderLog << std::endl;
            std::cerr.flush();

            throw std::runtime_error("[Shader] ERROR compiling shader:\n" + shaderLog);
        }
    }

    void link(GLuint handle)
    {
        glLinkProgram(handle);

        GLint result;
        glGetProgramiv(handle, GL_LINK_STATUS, &result);

        if(result == GL_FALSE)
        {
            GLint messageLength = 0;
            std::string programLog;

            glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &messageLength);

            if(messageLength > 0)
            {
                programLog.resize(static_cast<std::size_t>(messageLength));
                glGetProgramInfoLog(handle, messageLength, nullptr, &programLog[0]);

                std::cerr << "[Shader] ERROR link shaderprogram: \n" << programLog << std::endl;
                std::cerr.flush();
            }

            throw std::runtime_error((std::string("[Shader] ERROR link shaderprogram: \n") + programLog));
        }
    }
}

ShaderProgram shaderCreate(const std::string &vertexSource, const std::string &fragmentSource)
{
    ShaderProgram program{glCreateProgram(), glCreateShader(GL_VERTEX_SHADER), glCreateShader(GL_FRAGMENT_SHADER)};

    if(!program._vertexID || !program._fragmentID || !program.id)
    {
        std::cerr << "[Shader] Couldn't create shader program!" << std::endl;
        std::cerr.flush();
        throw std::runtime_error("[Shader] Couldn't create shader program!");
    }

    detail::compile(program._vertexID, vertexSource.c_str(), vertexSource.size());
    glAttachShader(program.id, program._vertexID);

    detail::compile(program._fragmentID, fragmentSource.c_str(), fragmentSource.size());
    glAttachShader(program.id, program._fragmentID);

    detail::link(program.id);

    return program;
}

ShaderProgram shaderLoad(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if(!vertexFile.is_open())
    {
        std::cerr << "[Shader] Couldn't open vertex shader file at " << std::endl;
        std::cerr.flush();
        throw std::runtime_error("[Shader] Couldn't open vertex shader file at " + vertexPath);
    }

    if(!fragmentFile.is_open())
    {
        std::cerr << "[Shader] Couldn't open fragment shader file at " << std::endl;
        std::cerr.flush();
        throw std::runtime_error("[Shader] Couldn't open fragment shader file at " + fragmentPath);
    }

    std::stringstream vertexSourceBuffer;
    vertexSourceBuffer << vertexFile.rdbuf();

    std::stringstream fragmentSourceBuffer;
    fragmentSourceBuffer << fragmentFile.rdbuf();

    return shaderCreate(vertexSourceBuffer.str(), fragmentSourceBuffer.str());
}

void shaderDelete(const ShaderProgram &program)
{
    glDetachShader(program.id, program._vertexID);
    glDetachShader(program.id, program._fragmentID);
    glDeleteShader(program._vertexID);
    glDeleteShader(program._fragmentID);

    glDeleteProgram(program.id);
}

void shaderUniform(ShaderProgram &shader, const std::string &name, const Matrix4D &value)
{
    GLint index = glGetUniformLocation(shader.id, name.c_str());
    if(index < 0)
    {
        std::cerr << "[Shader] Couldn't set value for uniform " << name << std::endl;
        std::cerr.flush();
        throw std::runtime_error("[Shader] Couldn't set value for uniform " + name);
    }
    glUniformMatrix4fv(index, 1, GL_FALSE, value.ptr());
}

void shaderUniform(ShaderProgram &shader, const std::string &name, int value)
{
    GLint index = glGetUniformLocation(shader.id, name.c_str());
    if(index < 0)
    {
        std::cerr << "[Shader] Couldn't set value for uniform " << name << std::endl;
        std::cerr.flush();
        throw std::runtime_error("[Shader] Couldn't set value for uniform " + name);
    }
    glUniform1i(index, value);
}
