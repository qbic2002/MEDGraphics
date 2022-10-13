//
// Created by danil on 23.09.2022.
//

#include "RootView.h"
#include "BgRenderer.h"
#include "ImageView.h"
#include "WindowBar.h"
#include "DropDownView.h"

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
        addChild(
                new DropDownView(context, Style{
                                         .position = {0, WINDOW_BAR_HEIGHT, 0, 0},
                                         .padding = padding(0)},
                                 {
                                         DropDownViewElementPrototype("Fast exit", []() {
                                             throw std::exception();
                                         }),
                                         DropDownViewElementPrototype("Save to", [&]() { this->context->saveImage(); })
                                 },
                                 Style{
                                         .position = {0, 0, FILL_PARENT, 24},
                                         .padding = padding(4),
                                         .background = Background{
                                                 .color = {60, 63, 63, 255},
                                                 .colorOnHover = {75, 110, 175, 255},
                                                 .colorOnPress = {66, 98, 155, 255}},
                                         .fontRenderer = assets::fontRenderer("assets/fonts/segoe-ui/Segoe UI.ttf",
                                                                              14)}));
    }
} // view