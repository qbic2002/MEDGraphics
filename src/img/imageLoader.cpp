//
// Created by danil on 18.09.2022.
//


#include "../utils/file.h"
#include "../stb_image.h"
#include "RGBAPixel.h"
#include "Raster.h"
#include "../pnm/utils/pnmUtil.h"
#include "imageLoader.h"

namespace img {
    Raster<RGBAPixel>* rgbaDataToRaster(const rgba* data, int width, int height) {
        auto* raster = new Raster<RGBAPixel>(width, height);
        for (unsigned int i = 0; i < width * height; i++) {
            raster->set(i, RGBAPixel(data[i]));
        }
        return raster;
    }

    bool isPNMSignature(const std::vector<char>& bytes) {
        return bytes.size() >= 2 && bytes[0] == 'P' && (bytes[1] == '4' || bytes[1] == '5' || bytes[1] == '6');
    }

    AbstractRaster* loadImageData(const std::string& fileName) {
        std::vector<char> bytes = utils::readAllBytes(fileName);
        return loadImageData(bytes);
    }

    AbstractRaster* loadImageData(const std::vector<char>& bytes) {
        if (isPNMSignature(bytes)) {
            auto pnmImage = pnm::readPNMImageFromMemory(const_cast<char*>(bytes.data()));
            return new Raster<RGBAPixel>(pnmImage.rgbaData);
        } else {
            int width, height, channels;
            unsigned char* data = stbi_load_from_memory((unsigned char*) bytes.data(), bytes.size(), &width, &height,
                                                        &channels, 0);

            auto* raster = rgbaDataToRaster(reinterpret_cast<const rgba*>(data), width, height);
            stbi_image_free(data);

            return raster;
        }
    }

}