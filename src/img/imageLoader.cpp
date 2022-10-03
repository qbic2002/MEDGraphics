//
// Created by danil on 18.09.2022.
//


#include "../utils/file.h"
#include "../stb_image.h"
#include "RGBAPixel.h"
#include "Raster.h"
#include "../pnm/utils/pnmUtil.h"
#include "imageLoader.h"
#include <cmath>

namespace img {

    std::vector<ustring> supportedSignatures({ // NOLINT(cert-err58-cpp)
                                                     {0xFF, 0xD8, 0xFF},
                                                     {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A},
                                                     (const unsigned char*) "BM",
                                                     (const unsigned char*) "P5",
                                                     (const unsigned char*) "P6"
                                             });

    Raster<RGBAPixel>* rgbaDataToRaster(const unsigned char* data, int width, int height, int channels) {
        auto* raster = new Raster<RGBAPixel>(width, height);
        if (channels == 3) {
            int pixelsCount = width * height;
            for (unsigned int i = 0; i < pixelsCount; i++) {
                raster->set(i, RGBAPixel(data[0], data[1], data[2], 255));
                data += channels;
            }
        } else {
            for (unsigned int i = 0; i < width * height * 4; i += 4) {
                raster->set(i / 4, RGBAPixel(data[i], data[i + 1], data[i + 2], data[i + 3]));
            }
        }
        return raster;
    }

    bool isPNMSignature(const std::vector<char>& bytes) {
        return bytes.size() >= 2 && bytes[0] == 'P' && (bytes[1] == '5' || bytes[1] == '6');
    }

    AbstractRaster* loadImageData(const std::string& fileName) {
        if (isImage(fileName))
            return loadImageData(utils::readAllBytes(fileName));

        return nullptr;
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

            if (data == nullptr)
                return nullptr;

            auto* raster = rgbaDataToRaster(data, width, height, channels);
            stbi_image_free(data);

            return raster;
        }
    }

    bool isImage(const std::string& fileName) {
        unsigned char* signatureBytes = utils::readNBytes(fileName, 8);
        if (signatureBytes == nullptr) return false;

        ustring signature(signatureBytes);

        delete signatureBytes;

        if (std::any_of(supportedSignatures.begin(), supportedSignatures.end(), [signature](ustring& supported) {
            return signature.starts_with(supported);
        }))
            return true;
        return false;
    }
}