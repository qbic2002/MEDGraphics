//
// Created by danil on 19.09.2022.
//

#include "Texture.h"
#include "../img/imageLoader.h"
#include "utils.h"

namespace gl {
    Texture::Texture(const std::string& fileName) {
        AbstractRaster* raster = img::loadImageData(fileName);
        textureId = loadTexture(raster, GL_CLAMP, GL_LINEAR, GL_NEAREST);
        delete raster;
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
}
