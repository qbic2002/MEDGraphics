//
// Created by danil on 16.09.2022.
//

#include <iostream>
#include "ShaderProgram.h"
#include "../utils/file.h"

void assertShaderCompiled(GLuint shaderId, const std::string& fileName) {
    GLint compiled;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shaderId, 1024, &log_length, message);
        std::cerr << "Couldn't compile shader '" << fileName << "': " << message << std::endl;
        throw std::exception();
    }
}

void assertProgramLinked(GLuint programId) {
    GLint programLinked;
    glGetProgramiv(programId, GL_LINK_STATUS, &programLinked);
    if (programLinked != GL_TRUE) {
        GLsizei log_length = 0;
        char message[1024];
        glGetProgramInfoLog(programId, 1024, &log_length, message);
        std::cerr << "Couldn't link program: " << message << std::endl;
        throw std::exception();
    }
}

GLuint readShaderFile(GLenum type, const std::string& fileName) {
    auto shaderSource = utils::readAllText(fileName);
    char* shaderSourceChars = shaderSource.data();

    GLuint shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &shaderSourceChars, nullptr);
    glCompileShader(shaderId);

    assertShaderCompiled(shaderId, fileName);
    return shaderId;
}

ShaderProgram::ShaderProgram(const std::string& fileNameVert, const std::string& fileNameFrag) {
    shaderVertId = readShaderFile(GL_VERTEX_SHADER, fileNameVert);
    shaderFragId = readShaderFile(GL_FRAGMENT_SHADER, fileNameFrag);

    programId = glCreateProgram();
    glAttachShader(programId, shaderVertId);
    glAttachShader(programId, shaderFragId);
    glLinkProgram(programId);

    assertProgramLinked(programId);
}

GLuint ShaderProgram::getUniformLocation(const GLchar* name) const {
    return glGetUniformLocation(programId, name);
};

void ShaderProgram::useProgram() const {
    glUseProgram(programId);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programId);
    glDeleteShader(shaderVertId);
    glDeleteShader(shaderFragId);
}