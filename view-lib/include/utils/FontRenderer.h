//
// Created by danil on 26.09.2022.
//

#ifndef MEDGRAPHICS_FONT_RENDERER_H
#define MEDGRAPHICS_FONT_RENDERER_H

#include <GL/glew.h>
#include "ft_error_check.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <filesystem>
#include "String.h"

namespace gl {

    struct GlyphData {
        /// In pixels
        int advanceX;
        int paddingLeft, paddingTop;
        unsigned width, height;
        /// In texture coords
        float texLeft, texRight, texTop, texBottom;
    };

    class FontRenderer {
    public:
        FontRenderer(const std::filesystem::path& fontFileName, unsigned fontSize);

        FontRenderer(const FontRenderer& other) = delete;

        FontRenderer(FontRenderer&& other) noexcept;

        ~FontRenderer();

        FontRenderer& operator=(const FontRenderer& other) = delete;

        FontRenderer& operator=(const FontRenderer&& other) = delete;

        float getTextWidth(const String& str) const;

        void renderText(const String& str, float x, float y);

        int getLineHeight() const;

    private:
        unsigned char* initTextureBitmap(const FT_Face& face);

        void fillTextureBitmap(const FT_Face& face, unsigned char* textureBmp);

        GLuint textureId = 0;
        int textureWidth, textureHeight;
        GlyphData* myGlyphData = nullptr;
        unsigned fontSize;
        int lineHeight;
    };

} // gl

#endif //MEDGRAPHICS_FONT_RENDERER_H
