//
// Created by danil on 18.09.2022.
//

#include "ImageView.h"
#include <GL/glew.h>
#include <cmath>

namespace view {

    void glVertexUV(float x, float y, float u, float v) {
        glTexCoord2f(u, v);
        glVertex2f(x, y);
    }

    ImageView::ImageView(MyApp* context, const Style& style) : View(context, style) {
        context->getImageFileStorage().addImageChangedListener([&]() {
            imageFitScreen();
        });
    }

    void ImageView::render() {
        auto* imageData = ((MyApp*) context)->getImageFileStorage().getCurImageFile();
        if (imageData == nullptr || imageData->textureId == 0)
            return;
        glBindTexture(GL_TEXTURE_2D, imageData->textureId);
        glPushMatrix();
        {
            glTranslatef(calculatedPos.x, calculatedPos.y, 0);

            glTranslatef(translateX, translateY, 0);
            glScalef(zoom, zoom, 1);

            glBegin(GL_QUADS);

            glVertexUV(0, 0, 0, 0);
            glVertexUV(0, imageData->raster->getHeight(), 0, 1);
            glVertexUV(imageData->raster->getWidth(), imageData->raster->getHeight(), 1, 1);
            glVertexUV(imageData->raster->getWidth(), 0, 1, 0);

            glEnd();
        }
        glPopMatrix();
    }

    bool ImageView::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {
        cursorX -= calculatedPos.x;
        cursorY -= calculatedPos.y;
        translateX -= cursorX;
        translateY -= cursorY;

        translateX /= zoom;
        translateY /= zoom;

        zoomOffset += yOffset;
        zoom = powf(1.5, zoomOffset);

        translateX *= zoom;
        translateY *= zoom;

        translateX += cursorX;
        translateY += cursorY;
        validateZoom();
        return true;
    }

    bool ImageView::onDrag(double x, double y, double dx, double dy) {
        translateX += dx;
        translateY += dy;
        validateZoom();
        return true;
    }

    void ImageView::onMeasure(const CalculatedPos& parentPos) {
        View::onMeasure(parentPos);
        imageFitScreen();
    }

    void ImageView::imageFitScreen() {
        auto* imageData = ((MyApp*) context)->getImageFileStorage().getCurImageFile();
        if (imageData == nullptr || imageData->textureId == 0)
            return;
        float horRatio = calculatedPos.width / imageData->raster->getWidth();
        float vertRatio = calculatedPos.height / imageData->raster->getHeight();
        setZoomRatio((vertRatio < horRatio) ? vertRatio : horRatio);
    }

    void ImageView::imageOriginalScale() {
        setZoomRatio(1);
    }

    void ImageView::validateZoom() {
        auto* imageData = ((MyApp*) context)->getImageFileStorage().getCurImageFile();
        if (imageData == nullptr || imageData->textureId == 0)
            return;
        float scaledRasterWidth = imageData->raster->getWidth() * zoom;
        float scaledRasterHeight = imageData->raster->getHeight() * zoom;
        if (scaledRasterWidth <= calculatedPos.width) {
            translateX = (calculatedPos.width - scaledRasterWidth) / 2;
        }
        if (scaledRasterHeight <= calculatedPos.height) {
            translateY = (calculatedPos.height - scaledRasterHeight) / 2;
        }
        if (translateX > calculatedPos.width / 2) {
            translateX = calculatedPos.width / 2;
        }
        if (translateY > calculatedPos.height / 2) {
            translateY = calculatedPos.height / 2;
        }
        if (translateX + scaledRasterWidth < calculatedPos.width / 2) {
            translateX = calculatedPos.width / 2 - scaledRasterWidth;
        }
        if (translateY + scaledRasterHeight < calculatedPos.height / 2) {
            translateY = calculatedPos.height / 2 - scaledRasterHeight;
        }
    }

    void ImageView::setZoomRatio(float ratio) {
        zoom = ratio;
        zoomOffset = logf(zoom) / logf(1.5);
        auto* imageFile = ((MyApp*) context)->getImageFileStorage().getCurImageFile();
        if (imageFile == nullptr || imageFile->textureId == 0)
            return;
        float scaledRasterWidth = imageFile->raster->getWidth() * ratio;
        float scaledRasterHeight = imageFile->raster->getHeight() * ratio;
        translateX = (calculatedPos.width - scaledRasterWidth) / 2;
        translateY = (calculatedPos.height - scaledRasterHeight) / 2;
    }

} // view