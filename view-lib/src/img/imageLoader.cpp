//
// Created by danil on 18.09.2022.
//


#include <img/imageLoader.h>
#include <utils/file.h>
#include <stb_image.h>
#include <img/PixelRGBA8.h>
#include <img/Raster.h>
#include "img/pnmUtils.h"
#include <cmath>

namespace img {
    typedef std::basic_string<unsigned char> ustring;

    std::vector<ustring> supportedSignatures({ // NOLINT(cert-err58-cpp)
                                                     {0xFF, 0xD8, 0xFF},
                                                     {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A},
                                                     (const unsigned char*) "BM",
                                                     (const unsigned char*) "P5",
                                                     (const unsigned char*) "P6"
                                             });

    ModernRaster* stbDataToRaster(const unsigned char* data, int width, int height, int channels) {
        ColorModelEnum colorModelEnum = COLOR_MODEL_RGB;
        switch (channels) {
            case 3:
                colorModelEnum = COLOR_MODEL_RGB;
                break;
            case 4:
                colorModelEnum = COLOR_MODEL_RGBA;
                break;
            default:
                throw std::runtime_error("unexpected channels count: " + std::to_string(channels));
                break;
        }

        int values = width * height * channels;
        auto pixels = std::shared_ptr<float[]>(new float[values]);
        for (int i = 0; i < values; i++)
            pixels[i] = data[i] / 255.0f;

        return new ModernRaster(width, height, pixels, colorModelEnum);
    }

    bool isPNMSignature(const std::vector<char>& bytes) {
        return bytes.size() >= 2 && bytes[0] == 'P' && (bytes[1] == '5' || bytes[1] == '6');
    }

    ModernRaster* loadImageData(const std::filesystem::path& file) {
        if (isImage(file)) {
            return loadImageData(utils::readAllBytes(file));
        }

        return nullptr;
    }

    ModernRaster* loadImageData(const std::vector<char>& bytes) {
        try {
            if (isPNMSignature(bytes)) {
                auto pnmImage = pnm::readPNMImageFromMemory(bytes.data(), bytes.size());
                return new ModernRaster(pnmImage.raster);
            } else {
                int width, height, channels;
                unsigned char* data = stbi_load_from_memory((unsigned char*) bytes.data(), bytes.size(), &width,
                                                            &height,
                                                            &channels, 0);

                if (data == nullptr)
                    return nullptr;

                auto* raster = stbDataToRaster(data, width, height, channels);
                stbi_image_free(data);

                return raster;
            }
        } catch (std::exception&) {
            return nullptr;
        }
    }

    bool isImage(const std::filesystem::path& file) noexcept {
        unsigned char* signatureBytes;
        try {
            signatureBytes = utils::readNBytes(file, 8);
        } catch (std::exception& e) {
            return false;
        }
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