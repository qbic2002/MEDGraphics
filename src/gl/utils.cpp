//
// Created by danil on 19.09.2022.
//

#include <iostream>
#include "utils.h"

namespace gl {

    void vertexUV(float x, float y, float u, float v) {
        glTexCoord2f(u, v);
        glVertex2f(x, y);
    }

    GLuint loadTexture(const unsigned char* data, int width, int height, GLint format, GLint wrap, GLint minFilter,
                       GLint magFilter) {

        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

//        GLenum err = glGetError();
//        std::cout << "error " << glewGetErrorString(err) << "\n";

        return textureId;
    }

    GLuint loadTexture(AbstractRaster* raster, GLint wrap, GLint minFilter, GLint magFilter) {
        return loadTexture(raster->getRgbaData(), raster->getWidth(), raster->getHeight(), GL_RGBA, wrap, minFilter,
                           magFilter);
    }
}