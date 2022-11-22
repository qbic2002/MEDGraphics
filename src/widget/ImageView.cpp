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
            invalidate();
        });
    }

    void ImageView::onDraw() {
        auto* imageData = ((MyApp*) context)->getImageFileStorage().getCurImageFile();
        if (imageData == nullptr || imageData->textureId == 0)
            return;
        glBindTexture(GL_TEXTURE_2D, imageData->textureId);
        glPushMatrix();
        {
            glTranslatef(edges.left, edges.top, 0);

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
        cursorX -= edges.left;
        cursorY -= edges.top;
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
        invalidate();
        return true;
    }

    bool ImageView::onDrag(double x, double y, double dx, double dy) {
        translateX += dx;
        translateY += dy;
        validateZoom();
        invalidate();
        return true;
    }

    void ImageView::onLayout(float left, float top, float right, float bottom) {
        imageFitScreen();
    }

    void ImageView::imageFitScreen() {
        auto* imageData = ((MyApp*) context)->getImageFileStorage().getCurImageFile();
        if (imageData == nullptr || imageData->textureId == 0)
            return;
        float horRatio = edges.width() / imageData->raster->getWidth();
        float vertRatio = edges.height() / imageData->raster->getHeight();
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
        if (scaledRasterWidth <= edges.width()) {
            translateX = (edges.width() - scaledRasterWidth) / 2;
        }
        if (scaledRasterHeight <= edges.height()) {
            translateY = (edges.height() - scaledRasterHeight) / 2;
        }
        if (translateX > edges.width() / 2) {
            translateX = edges.width() / 2;
        }
        if (translateY > edges.height() / 2) {
            translateY = edges.height() / 2;
        }
        if (translateX + scaledRasterWidth < edges.width() / 2) {
            translateX = edges.width() / 2 - scaledRasterWidth;
        }
        if (translateY + scaledRasterHeight < edges.height() / 2) {
            translateY = edges.height() / 2 - scaledRasterHeight;
        }
    }

    void ImageView::setZoomRatio(float ratio) {
        if (zoom == ratio)
            return;
        zoom = ratio;
        zoomOffset = logf(zoom) / logf(1.5);
        auto* imageFile = ((MyApp*) context)->getImageFileStorage().getCurImageFile();
        if (imageFile == nullptr || imageFile->textureId == 0)
            return;
        float scaledRasterWidth = imageFile->raster->getWidth() * ratio;
        float scaledRasterHeight = imageFile->raster->getHeight() * ratio;
        translateX = (edges.width() - scaledRasterWidth) / 2;
        translateY = (edges.height() - scaledRasterHeight) / 2;
        invalidate();
    }

} // view