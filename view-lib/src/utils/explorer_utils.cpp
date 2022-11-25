//
// Created by golov on 13.10.2022.
//

#include "utils/explorer_utils.h"
#include <windows.h>
#include "utils/logging.h"
#include <regex>
#include <filesystem>

void buildFilter() {
    if (!utils::filterString.empty()) {
        return;
    }
    std::wstring filter;
    for (const auto& saveFormat: utils::saveFormats) {
        std::wstring formatName = utils::imageFormat(saveFormat.format);
        filter
                .append(formatName)
                .push_back('\0');
        filter
                .append(L"*").append(saveFormat.extension).append(L"; ");

        std::wstring subformatFilters;
        std::wstring subformatString;
        for (const auto& subformat: saveFormat.subformats) {
            subformatFilters.append(L"*").append(subformat.second).append(L"; ");
            subformatString
                    .append(formatName).append(L" ").append(subformat.first).push_back('\0');
            subformatString
                    .append(L"*").append(subformat.second).push_back('\0');
        }

        filter.append(subformatFilters);
        filter.erase(filter.cend() - 2, filter.cend());
        filter.push_back('\0');
        filter.append(subformatString);
    }

    utils::filterString = filter;
}

bool utils::getOpenFileName(std::wstring& filename) {
    OPENFILENAMEW ofn;
    wchar_t szFile[300];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0PNM\0*.pnm; *.ppm; *.pgm\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    info("GetOpenFileName call");
    bool status = GetOpenFileNameW(&ofn);
    info("GetOpenFileName finished");
    filename = ofn.lpstrFile;
    return status;
}

utils::OpenFileInfo utils::getSaveFileName() {
    OPENFILENAMEW ofn;
    wchar_t szFile[300];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
//    ofn.lpstrFilter = L"PNM Color image\0*.ppm\0PNM Gray image\0*.pgm\0";
    buildFilter();
    ofn.lpstrFilter = utils::filterString.c_str();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_EXPLORER;

    info("GetOpenFileName call");
    bool status = GetOpenFileNameW(&ofn);
    info("GetOpenFileName finished");

    return {.isCancelled = !status, .filename = ofn.lpstrFile, .filterIndex = (int) ofn.nFilterIndex};
}

std::vector<std::wstring> getChildExtensions(const std::wstring& parentExtension) {
    std::vector<std::wstring> result;
    for (const auto& saveFormat: utils::saveFormats) {
        if (saveFormat.extension == parentExtension) {
            for (const auto& subformat: saveFormat.subformats) {
                result.push_back(subformat.second);
            }
        }
        return result;
    }
}

std::wstring utils::fixFileName(const std::wstring& filename, int filterIndex) {
    std::filesystem::path fn(filename);
    std::wstring extension = fn.extension().wstring();
    std::wstring newFilename = filename;

    --filterIndex;
    int index = 0;

    std::wstring parentExtension;
    std::wstring currentExtension;
    bool br = false;
    for (const auto& saveFormat: utils::saveFormats) {
        parentExtension = saveFormat.extension;
        currentExtension = saveFormat.extension;
        if (index == filterIndex) {
            break;
        }
        ++index;

        for (const auto& subformat: saveFormat.subformats) {
            currentExtension = subformat.second;
            if (index == filterIndex) {
                br = true;
                break;
            }
            ++index;
        }
        if (br)
            break;
    }

    std::vector<std::wstring> childExtensions = getChildExtensions(parentExtension);
    if (extension == parentExtension || extension == currentExtension ||
        (currentExtension == parentExtension && std::any_of(childExtensions.begin(), childExtensions.end(),
                                                            [extension](const std::wstring& ext) {
                                                                return ext == extension;
                                                            }))) {
        return filename;
    }


//    newFilename = filename.substr(0, filename.length() - extension.length());
    newFilename.append(currentExtension);

    return newFilename;
}
