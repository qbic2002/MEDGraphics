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

namespace gl {

    struct GlyphData {
        int index;
        unsigned int bmpWidth;
        unsigned int bmpHeight;
        int bmpLeft;
        int bmpTop;
        int advanceX;
    };

    class FontRenderer {
    public:
        FontRenderer(const std::string& fontFileName, int fontSize) : fontSize(fontSize) {
            ft::init(ftLibrary);

            FT_Face face = nullptr;
            ft::newFace(ftLibrary, fontFileName.c_str(), 0, &face);
            ft::setPixelSizes(face, 0, fontSize, fontFileName);

            charsCount = 192;
            textureWidth = nearestPow2(charsCount * fontSize);
            textureHeight = nearestPow2(fontSize);
            int nextCharIndex = 0;

            auto* textureData = new unsigned char[textureWidth * textureHeight];

            FT_UInt index;
            FT_ULong charCode = FT_Get_First_Char(face, &index);

            while (index) {
                ft::loadChar(face, charCode, FT_LOAD_RENDER);

                auto glyph = face->glyph;
                auto bmp = glyph->bitmap;

                for (int y = 0; y < bmp.rows; ++y) {
                    for (int x = 0; x < bmp.width; ++x) {
                        textureData[y * textureWidth + fontSize * nextCharIndex + x] = bmp.buffer[y * bmp.width + x];
                    }
                }

                myGlyphData[charCode] = {
                        .index = nextCharIndex,
                        .bmpWidth = bmp.width,
                        .bmpHeight = bmp.rows,
                        .bmpLeft = glyph->bitmap_left,
                        .bmpTop = glyph->bitmap_top,
                        .advanceX = (glyph->advance.x >> 6)
                };

                nextCharIndex++;

                std::cout << "Load char: " << (char) charCode << ", " << charCode << ", count: " << nextCharIndex
                          << ". bmp.width: " << bmp.width << ", divided metrics width: "
                          << glyph->metrics.width / fontSize << ", metrics.width: " << glyph->metrics.width
                          << std::endl;

                charCode = FT_Get_Next_Char(face, charCode, &index);
            }
            textureId = gl::loadTexture(textureData, textureWidth, textureHeight, GL_ALPHA, GL_CLAMP, GL_LINEAR,
                                        GL_NEAREST);
            delete[] textureData;

            ft::doneFace(face);
        }

        void renderText(const std::string& str, float x, float y) {
            glBindTexture(GL_TEXTURE_2D, textureId);
            glPushMatrix();
            glTranslatef(x, y, 0);
            for (const auto& c: str) {
                auto glyphData = myGlyphData[c];

                float left = glyphData.bmpLeft;
                float right = left + glyphData.bmpWidth;
                float top = -glyphData.bmpTop;
                float bottom = top + glyphData.bmpHeight;

                float texLeft = (float) glyphData.index * fontSize / textureWidth;
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

    private:
        template<typename T>
        T nearestPow2(const T num) {
            T nearest = 2;
            while ((nearest <<= 1) < num);
            return nearest;
        }

        FT_Library ftLibrary = nullptr;
        GLuint textureId = 0;
        int textureWidth, textureHeight;
        GlyphData myGlyphData[65536]{};
        int charsCount;
        int fontSize;
    };

} // gl

#endif //MEDGRAPHICS_FONTRENDERER_H
