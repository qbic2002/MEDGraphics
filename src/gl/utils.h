//
// Created by danil on 19.09.2022.
//

#ifndef MEDGRAPHICS_UTILS_H
#define MEDGRAPHICS_UTILS_H

#include <GL/glew.h>
#include "../img/AbstractRaster.h"

namespace gl {
    void vertexUV(float x, float y, float u, float v);

    GLint getMaxTextureSize();

    GLuint loadTexture(const unsigned char* data, int width, int height, GLint format, GLint wrap, GLint minFilter,
                       GLint magFilter);

    GLuint loadTexture(AbstractRaster* raster, GLint wrap, GLint minFilter, GLint magFilter);
}

#endif //MEDGRAPHICS_UTILS_H
