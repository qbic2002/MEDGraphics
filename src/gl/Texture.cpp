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

    Texture::~Texture() {
        glDeleteTextures(1, &textureId);
    }

    void Texture::bind() {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
