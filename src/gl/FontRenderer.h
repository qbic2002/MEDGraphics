//
// Created by danil on 26.09.2022.
//

#ifndef MEDGRAPHICS_FONTRENDERER_H
#define MEDGRAPHICS_FONTRENDERER_H

#include <string>
#include "GL/glew.h"
#include "utils.h"
#include "../utils/ft_error_check.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <iostream>
#include "../utils/file.h"

namespace gl {

    struct GlyphData {
        unsigned int bmpWidth;
        unsigned int bmpHeight;
        int bmpLeft;
        int bmpTop;
        int advanceX;
        unsigned int offset;
    };

    class FontRenderer {
    public:
        FontRenderer(const std::string& fontFileName, unsigned fontSize);

        FontRenderer(const FontRenderer& other) = delete;

        FontRenderer(FontRenderer&& other) noexcept;

        FontRenderer& operator=(const FontRenderer& other) = delete;

        FontRenderer& operator=(const FontRenderer&& other) = delete;

        float getTextWidth(const std::string& str) const;

        int getLineHeight() const;

        void renderText(const std::string& str, float x, float y);

        ~FontRenderer();

    private:
        void defineTextureSize(const FT_Face& face);

        void fillTextureBitmap(const FT_Face& face, unsigned char* textureBitMap);

        FT_Library ftLibrary = nullptr;
        GLuint textureId = 0;
        int textureWidth, textureHeight;
        GlyphData* myGlyphData = nullptr;
        int charsCount;
        unsigned fontSize;
        int lineHeight;
    };

} // gl

#endif //MEDGRAPHICS_FONTRENDERER_H
