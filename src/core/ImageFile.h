//
// Created by danil on 28.10.2022.
//

#ifndef MEDGRAPHICS_IMAGE_FILE_H
#define MEDGRAPHICS_IMAGE_FILE_H

#include <filesystem>
#include <GL/glew.h>
#include <img/AbstractRaster.h>

namespace fs = std::filesystem;

class ImageFile {
public:
    explicit ImageFile(const fs::path& path) noexcept;

    ImageFile(const ImageFile& other) = delete;

    ImageFile(ImageFile&& other) noexcept;

    ~ImageFile();

    void deleteRaster();

    void deleteCompRaster();

    void deleteTexture();

    void deleteCompTextureId();

    const fs::path& getPath() const;

    AbstractRaster* raster = nullptr;
    AbstractRaster* compRaster = nullptr;
    GLuint textureId = 0;
    GLuint compTextureId = 0;

private:
    fs::path path;
};

#endif //MEDGRAPHICS_IMAGE_FILE_H
