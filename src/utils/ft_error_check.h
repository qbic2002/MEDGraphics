//
// Created by danil on 26.09.2022.
//

#ifndef MEDGRAPHICS_FT_ERROR_CHECK_H
#define MEDGRAPHICS_FT_ERROR_CHECK_H

#include "freetype/freetype.h"
#include <string>

namespace ft {
    void init(FT_Library & library);

    void newFace(FT_Library library, const char* fileName, FT_Long faceIndex, FT_Face* aFace);

    void setPixelSizes(FT_Face face, FT_UInt pixelWidth, FT_UInt pixelHeight, const std::string& fileName);

    void loadChar(FT_Face face, FT_ULong char_code, FT_Int32 load_flags);

    void doneFace(FT_Face face);
}

#endif //MEDGRAPHICS_FT_ERROR_CHECK_H
