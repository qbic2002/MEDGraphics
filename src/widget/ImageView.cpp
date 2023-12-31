//
// Created by danil on 18.09.2022.
//

#include "ImageView.h"
#include <GL/glew.h>
#include <cmath>
#include <utils/gl_utils.h>

namespace view {

    int zoomToGridSize(float zoom) {
        float zoomBase2 = std::pow(2, (int) std::log2(zoom));
        if (zoomBase2 >= 16)
            return 1;
        return std::round(16 / zoomBase2);
    }

    ImageView::ImageView(Context* context, const ViewAttributes& attr) : View(context, attr) {}

    void ImageView::onDraw() {
        if (textureId == 0)
            return;
        glBindTexture(GL_TEXTURE_2D, textureId);
        glPushMatrix();
        {
            glTranslatef(innerEdges.left, innerEdges.top, 0);

            glTranslatef(translateX, translateY, 0);
            glScalef(zoom, zoom, 1);

            if (shouldRenderGrid) {
                gridShader.useProgram();
                glUniform1i(uniformSizeLoc, zoomToGridSize(zoom));
                glPositionQuad(0, 0, textureWidth, textureHeight);
                glUseProgram(0);
            }

            glTextureQuad(0, 0, textureWidth, textureHeight);
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

    void ImageView::onMouseMove(double x, double y) {
        View::onMouseMove(pointerX, pointerY);
        pointerX = (int) ((x - edges.left - translateX) / zoom);
        pointerY = (int) ((y - edges.top - translateY) / zoom);
    }

    void ImageView::onLayout(float left, float top, float right, float bottom) {
        validateZoom();
    }

    void ImageView::imageFitScreen() {
        if (textureId == 0)
            return;
        float horRatio = edges.width() / textureWidth;
        float vertRatio = edges.height() / textureHeight;
        setZoomRatio((vertRatio < horRatio) ? vertRatio : horRatio);
    }

    void ImageView::imageOriginalScale() {
        setZoomRatio(1);
    }

    void ImageView::validateZoom() {
        if (textureId == 0)
            return;
        float scaledRasterWidth = textureWidth * zoom;
        float scaledRasterHeight = textureHeight * zoom;
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
        if (textureId == 0)
            return;
        float scaledRasterWidth = textureWidth * ratio;
        float scaledRasterHeight = textureHeight * ratio;
        translateX = (edges.width() - scaledRasterWidth) / 2;
        translateY = (edges.height() - scaledRasterHeight) / 2;
        invalidate();
    }

    void ImageView::setTexture(int textureId, int textureWidth, int textureHeight) {
        if (this->textureId == textureId && this->textureWidth == textureWidth && this->textureHeight == textureHeight)
            return;
        this->textureId = textureId;
        this->textureWidth = textureWidth;
        this->textureHeight = textureHeight;
        imageFitScreen();
        invalidate();
    }

    int ImageView::getPointerX() const {
        return pointerX;
    }

    int ImageView::getPointerY() const {
        return pointerY;
    }

    void ImageView::setShouldRenderGrid(bool value) {
        if (shouldRenderGrid == value)
            return;
        shouldRenderGrid = value;
        invalidate();
    }

} // view