//
// Created by danil on 15.12.2022.
//

#ifndef MEDGRAPHICS_RASTER_COMPUTING_H
#define MEDGRAPHICS_RASTER_COMPUTING_H

#include "img/ModernColorModel.h"
#include "utils/gl_utils.h"

class RasterComputing {
public:
    explicit RasterComputing(const ModernRaster& raster) {
        int componentsCount = raster.getColorModel()->getComponentsCount();
        GLuint format;
        switch (componentsCount) {
            case 1:
                format = GL_RED;
                break;
            case 2:
                format = GL_RG;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                throw std::runtime_error("Unsupported components count");
        }
        width = raster.getWidth();
        height = raster.getHeight();
        imageTexture = gl::loadTexture(raster.getData(), format, GL_FLOAT, width, height, GL_RGBA32F,
                                       GL_CLAMP_TO_EDGE, GL_LINEAR, GL_NEAREST);
        previewTexture = gl::loadTexture(raster.getData(), format, GL_FLOAT, width, height, GL_RGBA32F,
                                         GL_CLAMP_TO_EDGE, GL_LINEAR, GL_NEAREST);
        rgbaTexture = gl::loadTexture(nullptr, GL_RGBA, GL_UNSIGNED_BYTE, width, height, GL_RGBA,
                                      GL_CLAMP_TO_EDGE, GL_LINEAR, GL_NEAREST);
    }

private:
    int width;
    int height;
    GLuint imageTexture;
    GLuint previewTexture;
    GLuint rgbaTexture;

    void copyImageToPreview() const {
        glBindImageTexture(0, imageTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindTexture(GL_TEXTURE_2D, previewTexture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 0, 0, width, height, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    }

    void copyPreviewToImage() const {
        glBindImageTexture(0, previewTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindTexture(GL_TEXTURE_2D, imageTexture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 0, 0, width, height, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    }
};

#endif //MEDGRAPHICS_RASTER_COMPUTING_H
