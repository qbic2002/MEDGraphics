//
// Created by danil on 26.09.2022.
//

#include "ft_error_check.h"
#include <iostream>

namespace ft {
    void init(FT_Library & library) {
        if (library == nullptr) {
            if (FT_Init_FreeType(&library) != 0) {
                std::cerr << "Unable to init FreeType Library" << std::endl;
                throw std::exception();
            }
        }
    }

    void newFace(FT_Library library, const char* fileName, FT_Long faceIndex, FT_Face* aFace) {
        int error = FT_New_Face(library, fileName, faceIndex, aFace);
        if (error == FT_Err_Unknown_File_Format) {
            std::cerr << "Error " << error << ": Unknown file format for font '" << fileName << "'" << std::endl;
            throw std::exception();
        } else if (error) {
            std::cerr << "Error " << error << ": Could not open font '" << fileName << "'" << std::endl;
            throw std::exception();
        }
    }

    void setPixelSizes(FT_Face face, FT_UInt pixelWidth, FT_UInt pixelHeight, const std::string& fileName) {
        int error = FT_Set_Pixel_Sizes(face, pixelWidth, pixelHeight);
        if (error) {
            std::cerr << "Error " << error << ": Could not set pixel sizes for font '" << fileName << "'" << std::endl;
            throw std::exception();
        }
    }

    void loadChar(FT_Face face, FT_ULong char_code, FT_Int32 load_flags) {
        int error = FT_Load_Char(face, char_code, load_flags);
        if (error) {
            std::cerr << "Error " << error << ": Could not load char '" << char_code << "'" << std::endl;
            throw std::exception();
        }
    }

    void doneFace(FT_Face face) {
        int error = FT_Done_Face(face);
        if (error) {
            std::cerr << "Error " << error << ": Could done face" << std::endl;
            throw std::exception();
        }
    }
}
