//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_BGRENDERER_H
#define MEDGRAPHICS_BGRENDERER_H

#include <GL/glew.h>
#include "../math/vec3.h"
#include "ShaderProgram.h"

namespace view {
    struct rotation {
        vec3 axis;
        float angle;
    };

    struct rectVertex {
    public:
        vec3 pos;
        vec3 velocity;
        rotation rotation;
        float rotationSpeed;
        unsigned char index;
    };

    class BgRenderer {
    public:
        void update(unsigned int width, unsigned int height);

        void render();

        ~BgRenderer();

    private:
        GLuint rectsVaoId = 0;
        GLuint rectsBufferId = 0;
        unsigned int rectsCount = 0;
        rectVertex* rectVertices = nullptr;
        unsigned int width = 0, height = 0;

        ShaderProgram rectsShader = ShaderProgram("assets/shaders/rects.vert", "assets/shaders/rects.frag");
        GLuint uniformThetaLoc = rectsShader.getUniformLocation("theta");
        GLuint uniformWidthLoc = rectsShader.getUniformLocation("width");
        GLuint uniformHeightLoc = rectsShader.getUniformLocation("height");

        void clear();

        void uploadVertexData();
    };
}


#endif //MEDGRAPHICS_BGRENDERER_H
