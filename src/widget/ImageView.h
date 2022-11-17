//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGEVIEW_H
#define MEDGRAPHICS_IMAGEVIEW_H

#include "utils/ShaderProgram.h"
#include "view/View.h"
#include "../core/MyApp.h"

namespace view {

    class ImageView : public View {
    public:
        ImageView(MyApp* context, const Style& style);

        ImageView(const ImageView& other) = delete;

        ImageView(const ImageView&& other) = delete;

        ImageView& operator=(const ImageView& other) = delete;

        ImageView& operator=(const ImageView&& other) = delete;

        void render() override;

        bool onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

        bool onDrag(double x, double y, double dx, double dy) override;

        void onMeasure(const CalculatedPos& parentPos) override;

        void imageFitScreen();

        void imageOriginalScale();

    private:
        void setZoomRatio(float ratio);

        void validateZoom();

        float zoom = 1;
        float zoomOffset = 0;
        float translateX = 0, translateY = 0;
    };

} // view

#endif //MEDGRAPHICS_IMAGEVIEW_H
