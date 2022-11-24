//
// Created by golov on 20.11.2022.
//

#ifndef MEDGRAPHICS_IMAGEFORMATS_H
#define MEDGRAPHICS_IMAGEFORMATS_H

#include <string>

namespace utils {

    enum ImageFormats {
        PNM,

    };

    ImageFormats imageFormat(const std::string& imageFormatString);

    std::wstring imageFormat(ImageFormats imageFormats);
}

#endif //MEDGRAPHICS_IMAGEFORMATS_H
