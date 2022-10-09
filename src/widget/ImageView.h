//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGEVIEW_H
#define MEDGRAPHICS_IMAGEVIEW_H

#include "../gl/ShaderProgram.h"
#include "../view/View.h"

namespace view {

    class ImageView : public View {
    public:
        ImageView(Context* context, const Style& style);

        ImageView(const ImageView& other) = delete;

        ImageView(const ImageView&& other) = delete;

        ImageView& operator=(const ImageView& other) = delete;

        ImageView& operator=(const ImageView&& other) = delete;

        void render() override;

        bool onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

        bool onDrag(double x, double y, double dx, double dy) override;

        void onMeasure(const CalculatedPos& parentPos) override;

        void initZoomWithImage();

    private:
        void validateZoom();

        float zoom = 1;
        float zoomOffset = 0;
        float translateX = 0, translateY = 0;
    };

} // view

#endif //MEDGRAPHICS_IMAGEVIEW_H
