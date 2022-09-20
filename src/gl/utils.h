//
// Created by danil on 19.09.2022.
//

#ifndef MEDGRAPHICS_UTILS_H
#define MEDGRAPHICS_UTILS_H

#include "GL/glew.h"
#include "../img/AbstractRaster.h"

namespace gl {
    GLuint loadTexture(AbstractRaster* raster, GLint wrap, GLint minFilter, GLint magFilter);
}

#endif //MEDGRAPHICS_UTILS_H
