//
// Created by danil on 18.09.2022.
//


#include "imageLoader.h"
#include "../utils/file.h"
#include "../stb_image.h"
#include "PixelRGBA8.h"
#include "Raster.h"
#include "../pnm/pnmUtils.h"
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

    Raster<PixelRGBA8>* rgbaDataToRaster(const unsigned char* data, int width, int height, int channels) {
        auto* raster = new Raster<PixelRGBA8>(width, height);
        if (channels == 3) {
            int pixelsCount = width * height;
            for (unsigned int i = 0; i < pixelsCount; i++) {
                raster->set(i, PixelRGBA8(data[0], data[1], data[2], 255));
                data += channels;
            }
        } else {
            for (unsigned int i = 0; i < width * height * 4; i += 4) {
                raster->set(i / 4, PixelRGBA8(data[i], data[i + 1], data[i + 2], data[i + 3]));
            }
        }
        return raster;
    }

    bool isPNMSignature(const std::vector<char>& bytes) {
        return bytes.size() >= 2 && bytes[0] == 'P' && (bytes[1] == '5' || bytes[1] == '6');
    }

    AbstractRaster* loadImageData(const std::filesystem::path& file) {
        if (isImage(file)) {
            return loadImageData(utils::readAllBytes(file));
        }

        return nullptr;
    }

    AbstractRaster* loadImageData(const std::vector<char>& bytes) {
        try {
            if (isPNMSignature(bytes)) {
                auto pnmImage = pnm::readPNMImageFromMemory(bytes.data(), bytes.size());
                return pnmImage.data->clone();
            } else {
                int width, height, channels;
                unsigned char* data = stbi_load_from_memory((unsigned char*) bytes.data(), bytes.size(), &width,
                                                            &height,
                                                            &channels, 0);

                if (data == nullptr)
                    return nullptr;

                auto* raster = rgbaDataToRaster(data, width, height, channels);
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