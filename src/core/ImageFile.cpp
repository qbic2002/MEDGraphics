//
// Created by danil on 28.10.2022.
//

#include "ImageFile.h"

ImageFile::ImageFile(const fs::path& path) noexcept: path(path) {}

ImageFile::ImageFile(ImageFile&& other) noexcept: path(other.path), raster(other.raster), compRaster(other.compRaster),
                                                  textureId(other.textureId), compTextureId(other.compTextureId) {
    other.raster = other.compRaster = nullptr;
    other.textureId = other.compTextureId = 0;
}

void ImageFile::deleteRaster() {
    if (raster) {
        delete raster;
        raster = nullptr;
    }
}

void ImageFile::deleteCompRaster() {
    if (compRaster) {
        delete compRaster;
        compRaster = nullptr;
    }
}

void ImageFile::deleteTexture() {
    if (textureId) {
        glDeleteTextures(1, &textureId);
        textureId = 0;
    }
}

void ImageFile::deleteCompTextureId() {
    if (compTextureId) {
        glDeleteTextures(1, &compTextureId);
        compTextureId = 0;
    }
}

ImageFile::~ImageFile() {
    deleteRaster();
    deleteCompRaster();
    deleteTexture();
    deleteCompTextureId();
}

const fs::path& ImageFile::getPath() const {
    return path;
}
