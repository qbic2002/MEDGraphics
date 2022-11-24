//
// Created by danil on 19.09.2022.
//

#ifndef MEDGRAPHICS_GL_UTILS_H
#define MEDGRAPHICS_GL_UTILS_H

#include <GL/glew.h>
#include "img/rgba.h"

namespace gl {
    void vertexUV(float x, float y, float u, float v);

    GLint getMaxTextureSize();

    GLuint loadTexture(const unsigned char* data, int width, int height, GLint format, GLint wrap, GLint minFilter,
                       GLint magFilter);
}

void glColor(const rgba& rgba);

void glPositionQuad(float x1, float y1, float x2, float y2);

void glTextureQuad(float x1, float y1, float x2, float y2);

#endif //MEDGRAPHICS_GL_UTILS_H
