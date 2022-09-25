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
        addChild(new ImageView(context, {.position = {0, WINDOW_BAR_HEIGHT, FILL_PARENT,
                                                      FILL_PARENT - Dimension(WINDOW_BAR_HEIGHT)}}));
        addChild(new WindowBar(context, {.position = {0, 0, FILL_PARENT, WINDOW_BAR_HEIGHT}}));
    }
} // view