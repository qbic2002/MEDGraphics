//
// Created by danil on 19.09.2022.
//

#include <utils/gl_utils.h>

namespace gl {
    void vertexUV(float x, float y, float u, float v) {
        glTexCoord2f(u, v);
        glVertex2f(x, y);
    }

    GLint getMaxTextureSize() {
        GLint size;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
        return size;
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

        return textureId;
    }
}

void glColor(const rgba& rgba) {
    glColor4ubv((GLubyte*) &rgba);
}

void glPositionQuad(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void glTextureQuad(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x1, y1);
    glTexCoord2f(1, 0);
    glVertex2f(x2, y1);
    glTexCoord2f(1, 1);
    glVertex2f(x2, y2);
    glTexCoord2f(0, 1);
    glVertex2f(x1, y2);
    glEnd();
}
