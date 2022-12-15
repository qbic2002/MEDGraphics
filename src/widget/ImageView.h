//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGEVIEW_H
#define MEDGRAPHICS_IMAGEVIEW_H

#include "view/View.h"
#include "../core/MyApp.h"
#include "utils/ShaderProgram.h"

namespace view {

    class ImageView : public View {
    public:
        ImageView(Context* context, const ViewAttributes& attr);

        ImageView(const ImageView& other) = delete;

        ImageView(const ImageView&& other) = delete;

        ImageView& operator=(const ImageView& other) = delete;

        ImageView& operator=(const ImageView&& other) = delete;

        void onDraw() override;

        bool onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

        bool onDrag(double x, double y, double dx, double dy) override;

        void onLayout(float left, float top, float right, float bottom) override;

        void imageFitScreen();

        void imageOriginalScale();

        void setTexture(int textureId, int textureWidth, int textureHeight);

        void onMouseMove(double x, double y) override;

        int getPointerX() const;

        int getPointerY() const;

        void setShouldRenderGrid(bool value);

    private:
        void setZoomRatio(float ratio);

        void validateZoom();

        float zoom = 1;
        float zoomOffset = 0;
        float translateX = 0, translateY = 0;
        int textureWidth = 0, textureHeight = 0;
        int textureId;
        int pointerX = 0, pointerY = 0;
        bool shouldRenderGrid = false;

        ShaderProgram gridShader = ShaderProgram("grid");
        GLuint uniformSizeLoc = gridShader.getUniformLocation("size");
    };

} // view

#endif //MEDGRAPHICS_IMAGEVIEW_H
