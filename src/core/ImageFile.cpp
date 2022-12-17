//
// Created by danil on 28.10.2022.
//

#include "ImageFile.h"

ImageFile::ImageFile(const fs::path& path) noexcept: path(path) {}

ImageFile::ImageFile(ImageFile&& other) noexcept:
        path(std::move(other.path)), raster(other.raster), texture(other.texture),
        shouldBeLoaded(other.shouldBeLoaded), isRasterLoaded(other.isRasterLoaded),
        isTextureLoaded(other.isTextureLoaded), isLoadFailed(other.isLoadFailed) {
    other.raster = nullptr;
    other.texture = nullptr;
}

void ImageFile::deleteRaster() {
    if (raster) {
        delete raster;
        raster = nullptr;
    }
}

void ImageFile::deleteTexture() {
    delete texture;
    texture = nullptr;
}

ImageFile::~ImageFile() {
    deleteRaster();
    deleteTexture();
}

const fs::path& ImageFile::getPath() const {
    return path;
}

bool ImageFile::loadRaster() {
    raster = img::loadImageData(path);
    return raster != nullptr;
}

void ImageFile::loadTexture() {
    delete texture;
    texture = new gl::Texture(*raster);
}

void ImageFile::verifyRasterLoaded() {
    if (isLoadFailed || (shouldBeLoaded == isRasterLoaded))
        return;

    if (shouldBeLoaded) {
        utils::TimeStamp timeStamp;
        info() << "start to load image data " << path;
        isRasterLoaded = loadRaster();
        isLoadFailed = !isRasterLoaded;
        if (isRasterLoaded) {
            info() << "load image data " << path << " took " << timeStamp.ms() << " ms";
        } else {
            info() << "load image data " << path << " failed";
        }
    } else {
        deleteRaster();
        isRasterLoaded = false;
    }
}

void ImageFile::verifyTextureLoaded() {
    if (isLoadFailed || (shouldBeLoaded == isTextureLoaded))
        return;

    if (shouldBeLoaded) {
        if (isRasterLoaded) {
            info() << "load texture " << path;
            loadTexture();
            isTextureLoaded = true;
        }
    } else {
        info() << "unload texture " << path;
        deleteTexture();
        isTextureLoaded = false;
    }
}
