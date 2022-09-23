//
// Created by danil on 23.09.2022.
//

#include "RootView.h"
#include "BgRenderer.h"
#include "ImageView.h"
#include "WindowBar.h"

namespace view {
    RootView::RootView(Context* context, const Style& style) : ViewGroup(context, style) {
        addChild(new BgRenderer(context));
        addChild(new ImageView(context));
        addChild(new WindowBar(context, {.position = {0, 0, (unsigned) utils::getWindowWidth(), 28}}));

        setOnWindowResizeListener([](View& view, unsigned width, unsigned height) {
            view.getStyle().position = {0, 0, width, height};
        });
    }
} // view