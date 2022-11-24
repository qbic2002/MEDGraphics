//
// Created by golov on 20.11.2022.
//

#include "utils/ImageFormats.h"

std::wstring utils::imageFormat(utils::ImageFormats imageFormats) {
    switch (imageFormats) {
        case ImageFormats::PNM:
            return L"PNM";
    }

    throw std::exception();
}

utils::ImageFormats utils::imageFormat(const std::string& imageFormatString) {
    if (imageFormatString == "PNM") {
        return ImageFormats::PNM;
    }

    throw std::exception();
}
