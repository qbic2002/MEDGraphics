//
// Created by danil on 26.09.2022.
//

#include "FontRenderer.h"

namespace gl {
    template<typename T>
    T nearestPow2(const T num) {
        T nearest = 2;
        while ((nearest <<= 1) < num);
        return nearest;
    }

    FontRenderer::FontRenderer(const std::string& fontFileName, unsigned int fontSize) : fontSize(fontSize) {
        ft::init(ftLibrary);
        setTextureSize(fontFileName, fontSize);
        auto* textureData = new unsigned char[textureWidth * textureHeight];

        fillTextureBitmap(fontFileName, fontSize, textureData);

        textureId = gl::loadTexture(textureData, textureWidth, textureHeight, GL_ALPHA, GL_CLAMP, GL_LINEAR,
                                    GL_NEAREST);
        delete[] textureData;
    }

    FontRenderer::FontRenderer(FontRenderer&& other) noexcept: ftLibrary(other.ftLibrary), textureId(other.textureId),
                                                               textureWidth(other.textureWidth),
                                                               textureHeight(other.textureHeight),
                                                               charsCount(other.charsCount),
                                                               fontSize(other.fontSize) {
        other.textureId = 0;
        for (int i = 0; i < 65536; i++) {
            myGlyphData[i] = other.myGlyphData[i];
        }
    }

    void FontRenderer::renderText(const std::string& str, float x, float y) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glPushMatrix();
        glTranslatef(x, y + fontSize, 0);
        for (const auto& c: str) {
            auto glyphData = myGlyphData[c];

            float left = glyphData.bmpLeft;
            float right = left + glyphData.bmpWidth;
            float top = -glyphData.bmpTop;
            float bottom = top + glyphData.bmpHeight;

            float texLeft = (float) glyphData.offset / textureWidth;
            float texRight = texLeft + (float) glyphData.bmpWidth / textureWidth;
            float texTop = 0;
            float textBottom = (float) glyphData.bmpHeight / textureHeight;

            glBegin(GL_QUADS);
            {
                gl::vertexUV(left, top, texLeft, texTop);
                gl::vertexUV(left, bottom, texLeft, textBottom);
                gl::vertexUV(right, bottom, texRight, textBottom);
                gl::vertexUV(right, top, texRight, texTop);
            }
            glEnd();

            glTranslatef(glyphData.advanceX, 0, 0);
        }

        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    FontRenderer::~FontRenderer() {
        if (textureId != 0)
            glDeleteTextures(1, &textureId);
    }

    void FontRenderer::setTextureSize(const std::string& fontFileName, unsigned int fontSize) {
        textureHeight = nearestPow2(fontSize);

        FT_Face face = nullptr;
        ft::newFace(ftLibrary, fontFileName.c_str(), 0, &face);
        ft::setPixelSizes(face, 0, fontSize, fontFileName);

        FT_UInt index = 0;
        FT_ULong charCode = FT_Get_First_Char(face, &index);

        unsigned int offset = 0;

        while (index) {
            ft::loadChar(face, charCode, FT_LOAD_RENDER);
            auto glyph = face->glyph;
            auto charBitMap = face->glyph->bitmap;
            textureWidth += charBitMap.width;

            myGlyphData[charCode] = {
                    .bmpWidth = charBitMap.width,
                    .bmpHeight = charBitMap.rows,
                    .bmpLeft = glyph->bitmap_left,
                    .bmpTop = glyph->bitmap_top,
                    .advanceX = (glyph->advance.x >> 6),
                    .offset = offset
            };

            std::cout << "char: \"" << (char) charCode << "\" width: " << charBitMap.width << std::endl;

            offset += charBitMap.width;
            charCode = FT_Get_Next_Char(face, charCode, &index);
        }

        textureWidth = nearestPow2(textureWidth);
        ft::doneFace(face);
    }

    void FontRenderer::fillTextureBitmap(const std::string& fontFileName, unsigned int fontSize,
                                         unsigned char* textureBitMap) {
        FT_Face face = nullptr;
        ft::newFace(ftLibrary, fontFileName.c_str(), 0, &face);
        ft::setPixelSizes(face, 0, fontSize, fontFileName);

        FT_UInt index = 0;
        FT_ULong charCode = FT_Get_First_Char(face, &index);

        while (index) {
            ft::loadChar(face, charCode, FT_LOAD_RENDER);

            auto glyph = face->glyph;
            auto bmp = glyph->bitmap;

            for (int y = 0; y < bmp.rows; ++y) {
                for (int x = 0; x < bmp.width; ++x) {
                    textureBitMap[y * textureWidth + myGlyphData[charCode].offset + x] = bmp.buffer[y * bmp.width + x];
                }
            }

            std::cout << "Load char: " << (char) charCode << ", " << charCode << ". bmp.width: " << bmp.width
                      << ", divided metrics width: "
                      << glyph->metrics.width / fontSize << ", metrics.width: " << glyph->metrics.width
                      << std::endl;

            charCode = FT_Get_Next_Char(face, charCode, &index);
        }

        ft::doneFace(face);
    }


} // gl