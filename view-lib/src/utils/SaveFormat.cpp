//
// Created by danil on 14.12.2022.
//

#include "utils/SaveFormat.h"
#include "img/pnmUtils.h"
#include "img/pngUtils.h"
#include "img/PNMImage.h"

namespace utils {
    std::wstring SaveFormat::toFilterString() const {
        std::wstring result = label;
        result.push_back('\0');
        result.append(L"*.").append(defaultExtension);
        for (auto& ext: possibleExtensions) {
            result.append(L"; *.").append(ext);
        }
        result.push_back('\0');

        return std::move(result);
    }

    void SaveFormat::saveImage(const ModernRaster* raster, std::wstring& enteredFileName) const {
        fixFileName(enteredFileName);
        saveFunction(raster, enteredFileName);
    }

    void SaveFormat::fixFileName(std::wstring& fileName) const {
        std::filesystem::path file(fileName);
        std::wstring extension = file.extension().wstring();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        if (extension == defaultExtension)
            return;
        for (const auto& ext: possibleExtensions) {
            if (extension == ext)
                return;
        }
        fileName.append(L".").append(defaultExtension);
    }

    const std::vector<SaveFormat> saveFormats{ // NOLINT(cert-err58-cpp)
            SaveFormat{L"Color PNM", {L"pnm"}, L"ppm",
                       [](const ModernRaster* img, const std::filesystem::path& path) {
                           pnm::writePNMImage(PNMImage(*img), path);
                       }},
            SaveFormat{L"Gray PNM", {L"pnm"}, L"pgm",
                       [](const ModernRaster* img, const std::filesystem::path& path) {
                           pnm::writePNMImage(PNMImage(*img), path);
                       }},
            SaveFormat{L"PNG", {}, L"png",
                       [](const ModernRaster* img, const std::filesystem::path& path) {
                           png::writePNGImage(*img, path);
                       }},
    };

    std::wstring saveFilterString;

    const wchar_t* getSaveFilterString() {
        if (!saveFilterString.empty()) {
            return saveFilterString.c_str();
        }

        for (const auto& saveFormat: saveFormats) {
            saveFilterString.append(saveFormat.toFilterString());
        }
        return saveFilterString.c_str();
    }

    const SaveFormat& getSaveFormat(int index) {
        return saveFormats[index];
    }
} // utils