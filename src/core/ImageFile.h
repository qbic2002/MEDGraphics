//
// Created by danil on 28.10.2022.
//

#ifndef MEDGRAPHICS_IMAGE_FILE_H
#define MEDGRAPHICS_IMAGE_FILE_H

#include <filesystem>
#include <GL/glew.h>
#include <img/AbstractRaster.h>
#include "img/ModernColorModel.h"
#include "utils/measureTime.h"
#include "utils/logging.h"
#include "img/imageLoader.h"
#include "utils/gl_utils.h"
#include "utils/Texture.h"

namespace fs = std::filesystem;

class ImageFile {
public:
    explicit ImageFile(const fs::path& path) noexcept;

    ImageFile(const ImageFile& other) = delete;

    ImageFile(ImageFile&& other) noexcept;

    ~ImageFile();

    /**
     * @return bool is operation successful
     */
    bool loadRaster();

    void loadTexture();

    void deleteRaster();

    void deleteTexture();

    const fs::path& getPath() const;

    ModernRaster* raster = nullptr;
    gl::Texture* texture = nullptr;

    bool shouldBeLoaded = false;

    void verifyRasterLoaded();

    void verifyTextureLoaded();

private:
    fs::path path;
    bool isRasterLoaded = false;
    bool isTextureLoaded = false;
    bool isLoadFailed = false;
};

#endif //MEDGRAPHICS_IMAGE_FILE_H
