//
// Created by danil on 16.09.2022.
//

#ifndef MEDGRAPHICS_SHADERPROGRAM_H
#define MEDGRAPHICS_SHADERPROGRAM_H

#include <string>
#include "GL/glew.h"

class ShaderProgram {

public:
    ShaderProgram(const std::string& fileNameVert, const std::string& fileNameFrag);

    [[nodiscard]] GLuint getProgramId() const;

    ~ShaderProgram();

private:
    GLuint programId;
    GLuint shaderVertId;
    GLuint shaderFragId;
};


#endif //MEDGRAPHICS_SHADERPROGRAM_H
