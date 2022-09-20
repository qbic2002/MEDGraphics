//
// Created by danil on 19.09.2022.
//

#include "utils.h"

namespace gl {
    GLuint loadTexture(AbstractRaster* raster, GLint wrap, GLint minFilter, GLint magFilter) {
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raster->getWidth(), raster->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     raster->getRgbaData());
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureId;
    }
}