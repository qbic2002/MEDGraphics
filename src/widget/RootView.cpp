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
        auto* imageView = new ImageView(context, Style{
                .position = {0, WINDOW_BAR_HEIGHT, FILL_PARENT, FILL_PARENT - WINDOW_BAR_HEIGHT},
                .isDraggable = true});
        addChild(imageView);
        addChild(new WindowBar(
                context,
                Style{
                        .position = {0, 0, FILL_PARENT, WINDOW_BAR_HEIGHT},
                        .background = Background()
                                .setColor(rgba{0, 0, 0, 255})
                                .setColorOnPress(rgba{0, 0, 0, 255})
                                .setColorOnHover(rgba{0, 0, 0, 255})},
                imageView));
    }
} // view