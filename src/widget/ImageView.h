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

        ~ImageView() override;

    private:
        ShaderProgram* shader = new ShaderProgram("default");
    };

} // view

#endif //MEDGRAPHICS_IMAGEVIEW_H
