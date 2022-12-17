//
// Created by danil on 18.09.2022.
//

#include <img/imageLoader.h>
#include <utils/file.h>
#include <stb_image.h>
#include <img/Raster.h>
#include "img/pnmUtils.h"
#include "img/pngUtils.h"
#include "img/format/jpeg_utils.h"
#include <cmath>

namespace img {

    typedef std::basic_string<unsigned char> ustring;

    struct ImageFormat {
        std::wstring name;
        ustring signature;

        bool equalsSignature(const unsigned char* bytes) const {
            for (int i = 0; i < signature.length(); i++) {
                if (signature[i] != bytes[i])
                    return false;
            }
            return true;
        }

        ModernRaster* (* read)(const char* data, int length);
    };

    ModernRaster* readPng(const char* data, int length) {
        return new ModernRaster(png::readPNGImageFromMemory(data, length));
    }

    ModernRaster* readPnm(const char* data, int length) {
        auto pnmImage = pnm::readPNMImageFromMemory(data, length);
        return new ModernRaster(pnmImage.raster);
    }

    ModernRaster* readStb(const char* data, int length) {
        int width, height, channels;
        unsigned char* pixels = stbi_load_from_memory((unsigned char*) data, length, &width, &height, &channels, 0);
        if (data == nullptr)
            throw std::runtime_error("stb_image return null");

        auto* raster = ModernRaster::fromBytesArray(pixels, width, height, channels);
        stbi_image_free(pixels);
        return raster;
    }

    const std::vector<ImageFormat> imageFormats = {
            ImageFormat{L"PGM", (const unsigned char*) "P5", readPnm},
            ImageFormat{L"PPM", (const unsigned char*) "P6", readPnm},
            ImageFormat{L"PNG", {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}, readPng},
            ImageFormat{L"JPEG", {0xFF, 0xD8}, img::readJpegImage},
//            ImageFormat{L"JPEG", {0xFF, 0xD8}, readStb},
            ImageFormat{L"BMP", (const unsigned char*) "BM", readStb},
    };

    ModernRaster* loadImageData(const std::filesystem::path& file) {
        try {
            auto signature = std::unique_ptr<unsigned char>(utils::readNBytes(file, 8));
            if (signature == nullptr)
                return nullptr;
            for (auto& format: imageFormats) {
                if (format.equalsSignature(signature.get())) {
                    std::vector<char> bytes = utils::readAllBytes(file);
                    return format.read(bytes.data(), bytes.size());
                }
            }
            return nullptr;
        } catch (const std::exception& e) {
            return nullptr;
        }
    }

    bool isImage(const std::filesystem::path& file) noexcept {
        try {
            auto signature = std::unique_ptr<unsigned char>(utils::readNBytes(file, 8));
            if (signature == nullptr)
                return false;
            for (auto& format: imageFormats) {
                if (format.equalsSignature(signature.get()))
                    return true;
            }
            return false;
        } catch (const std::exception& e) {
            return false;
        }
    }
}