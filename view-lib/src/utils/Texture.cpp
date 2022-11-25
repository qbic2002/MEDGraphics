//
// Created by danil on 19.09.2022.
//

#include <utils/Texture.h>
#include <img/imageLoader.h>
#include <utils/gl_utils.h>

namespace gl {
    Texture::Texture(const ModernRaster& raster) {
        width = raster.getWidth();
        height = raster.getHeight();
        textureId = loadTexture(raster.getRgbaData(), width, height, GL_RGBA, GL_CLAMP, GL_LINEAR, GL_NEAREST);
    }

    Texture::Texture(Texture&& other) noexcept: textureId(other.textureId) {
        other.textureId = 0;
    };

    Texture::~Texture() {
        if (textureId == 0)
            return;
        glDeleteTextures(1, &textureId);
    }

    void Texture::bind() const {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void Texture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint Texture::getWidth() const {
        return width;
    }

    GLuint Texture::getHeight() const {
        return height;
    }

    GLuint Texture::getTextureId() const {
        return textureId;
    }

    void Texture::update(const ModernRaster& raster) {
        width = raster.getWidth();
        height = raster.getHeight();
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raster.getRgbaData());
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
