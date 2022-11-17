//
// Created by danil on 26.09.2022.
//

#include <utils/FontRenderer.h>
#include <utils/gl_utils.h>
#include <cmath>

namespace gl {
    template<typename T>
    constexpr inline T nearestPow2(const T num) {
        T nearest = 2;
        while ((nearest <<= 1) < num);
        return nearest;
    }

    FontRenderer::FontRenderer(const std::filesystem::path& fontFileName, unsigned int fontSize) : fontSize(fontSize) {
        myGlyphData = (GlyphData*) malloc(65536 * sizeof(GlyphData));

        FT_Library ftLibrary = nullptr;
        ft::init(ftLibrary);

        FT_Face face = nullptr;
        std::string fileNameStr = fontFileName.string();
        ft::newFace(ftLibrary, fileNameStr.c_str(), 0, &face);
        ft::setPixelSizes(face, 0, fontSize, fileNameStr);
        lineHeight = roundf(((float) face->height / face->units_per_EM) * fontSize);

        auto* textureData = initTextureBitmap(face);
        fillTextureBitmap(face, textureData);

        ft::doneFace(face);

        textureId = gl::loadTexture(textureData, textureWidth, textureHeight, GL_ALPHA, GL_CLAMP, GL_LINEAR,
                                    GL_NEAREST);
        delete[] textureData;
    }

    FontRenderer::FontRenderer(FontRenderer&& other) noexcept: textureId(other.textureId),
                                                               textureWidth(other.textureWidth),
                                                               textureHeight(other.textureHeight),
                                                               fontSize(other.fontSize),
                                                               lineHeight(other.lineHeight),
                                                               myGlyphData(other.myGlyphData) {
        other.textureId = 0;
        other.myGlyphData = nullptr;
    }

    FontRenderer::~FontRenderer() {
        if (textureId != 0)
            glDeleteTextures(1, &textureId);

        free(myGlyphData);
    }

    float FontRenderer::getTextWidth(const String& str) const {
        float width = 0;
        for (const auto& c: str) {
            width += myGlyphData[c].advanceX;
        }

        return width;
    }

    void FontRenderer::renderText(const String& str, float x, float y) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glPushMatrix();
        glTranslatef(x, y + fontSize, 0);
        for (const auto c: str) {
            auto glyphData = myGlyphData[c];

            float left = glyphData.paddingLeft;
            float right = left + glyphData.width;
            float top = -glyphData.paddingTop;
            float bottom = top + glyphData.height;

            glBegin(GL_QUADS);
            {
                gl::vertexUV(left, top, glyphData.texLeft, glyphData.texTop);
                gl::vertexUV(left, bottom, glyphData.texLeft, glyphData.texBottom);
                gl::vertexUV(right, bottom, glyphData.texRight, glyphData.texBottom);
                gl::vertexUV(right, top, glyphData.texRight, glyphData.texTop);
            }
            glEnd();

            glTranslatef(glyphData.advanceX, 0, 0);
        }

        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned char* FontRenderer::initTextureBitmap(const FT_Face& face) {
        int maxTextureSize = getMaxTextureSize();

        int curTextureHeight = 0;
        int curTextureWidth = 0;

        int curLineHeight = 0;
        int curLineWidth = 0;

        FT_UInt index = 0;
        FT_ULong charCode = FT_Get_First_Char(face, &index);

        while (index) {
            ft::loadChar(face, charCode, FT_LOAD_RENDER);
            auto bmp = face->glyph->bitmap;

            if (curLineWidth + bmp.width > maxTextureSize) {
                curTextureHeight += curLineHeight;
                if (curLineWidth > curTextureWidth) {
                    curTextureWidth = curLineWidth;
                }

                curLineWidth = bmp.width;
                curLineHeight = bmp.rows;
            } else {
                curLineWidth += bmp.width;
                if (curLineHeight < bmp.rows) {
                    curLineHeight = bmp.rows;
                }
            }
            charCode = FT_Get_Next_Char(face, charCode, &index);
        }

        curTextureHeight += curLineHeight;
        if (curTextureWidth < curLineWidth) {
            curTextureWidth = curLineWidth;
        }

        textureWidth = nearestPow2(curTextureWidth);
        textureHeight = nearestPow2(curTextureHeight);
        return new unsigned char[textureWidth * textureHeight];
    }


    void copy(const unsigned char* src, int srcWidth, int srcX, int srcY,
              unsigned char* dst, int dstWidth, int dstX, int dstY,
              int width, int height) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                dst[(dstY + y) * dstWidth + dstX + x] = src[(srcY + y) * srcWidth + srcX + x];
            }
        }
    }

    void FontRenderer::fillTextureBitmap(const FT_Face& face, unsigned char* textureBmp) {
        int curTextureHeight = 0;
        int curTextureWidth = 0;

        int curLineHeight = 0;
        int curLineWidth = 0;

        FT_UInt index = 0;
        FT_ULong charCode = FT_Get_First_Char(face, &index);

        while (index) {
            ft::loadChar(face, charCode, FT_LOAD_RENDER);
            auto glyph = face->glyph;
            auto bmp = glyph->bitmap;

            if (curLineWidth + bmp.width > textureWidth) {
                curTextureHeight += curLineHeight;
                if (curLineWidth > curTextureWidth) {
                    curTextureWidth = curLineWidth;
                }

                curLineWidth = bmp.width;
                curLineHeight = bmp.rows;
            } else {
                curLineWidth += bmp.width;
                if (curLineHeight < bmp.rows) {
                    curLineHeight = bmp.rows;
                }
            }

            copy(bmp.buffer, bmp.width, 0, 0,
                 textureBmp, textureWidth, curLineWidth - bmp.width, curTextureHeight,
                 bmp.width, bmp.rows);

            myGlyphData[charCode] = {
                    .advanceX = glyph->advance.x >> 6,
                    .paddingLeft = glyph->bitmap_left,
                    .paddingTop = glyph->bitmap_top,
                    .width = bmp.width,
                    .height = bmp.rows,
                    .texLeft = (float) (curLineWidth - bmp.width) / textureWidth,
                    .texRight = (float) curLineWidth / textureWidth,
                    .texTop = (float) curTextureHeight / textureHeight,
                    .texBottom = (float) (curTextureHeight + bmp.rows) / textureHeight,
            };

            charCode = FT_Get_Next_Char(face, charCode, &index);
        }
    }

    int FontRenderer::getLineHeight() const {
        return lineHeight;
    }


} // gl