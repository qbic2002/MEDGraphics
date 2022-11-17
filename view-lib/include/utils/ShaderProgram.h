//
// Created by danil on 16.09.2022.
//

#ifndef MEDGRAPHICS_SHADERPROGRAM_H
#define MEDGRAPHICS_SHADERPROGRAM_H

#include <string>
#include <GL/glew.h>

class ShaderProgram {

public:
    explicit ShaderProgram(const std::string& name);

    GLuint getUniformLocation(const GLchar* name) const;

    void useProgram() const;

    ~ShaderProgram();

private:
    GLuint programId;
    GLuint shaderVertId;
    GLuint shaderFragId;
};


#endif //MEDGRAPHICS_SHADERPROGRAM_H
