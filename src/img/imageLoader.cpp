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
    Raster<RGBAPixel>* rgbaDataToRaster(const unsigned char* data, int width, int height, int channels) {
        auto* raster = new Raster<RGBAPixel>(width, height);
        if (channels == 3) {
            for (unsigned int i = 0; i < width * height * channels; i += channels) {
                raster->set(i / channels, RGBAPixel(data[i], data[i + 1], data[i + 2], 255));
            }
        } else {
            for (unsigned int i = 0; i < width * height * 4; i += 4) {
                raster->set(i / 4, RGBAPixel(data[i], data[i + 1], data[i + 2], data[i + 3]));
            }
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
            switch (pnmImage.data->getPixelType()) {
                case RGBA: {
                    return new Raster(*((Raster<RGBAPixel>*) pnmImage.data));
                }
                case GRAY: {
                    return new Raster(*((Raster<GrayPixel>*) pnmImage.data));
                }
                default : {
                    throw std::exception();
                }
            }
        } else {
            int width, height, channels;
            unsigned char* data = stbi_load_from_memory((unsigned char*) bytes.data(), bytes.size(), &width, &height,
                                                        &channels, 0);

            auto* raster = rgbaDataToRaster(data, width, height, channels);
            stbi_image_free(data);

            return raster;
        }
    }
}