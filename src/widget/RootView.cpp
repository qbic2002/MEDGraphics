//
// Created by danil on 23.09.2022.
//

#include "RootView.h"
#include "BgRenderer.h"
#include "ImageView.h"
#include "WindowBar.h"
#include "DropDownView.h"
#include "MessageView.h"

namespace view {
    RootView::RootView(Context* context, const Style& style) : ViewGroup(context, style) {
        addChild(new BgRenderer(context));
        auto* imageView = new ImageView(context, Style().edit([](Style& style) {
            style.isDraggable = true;
            style.set({.position = {0, WINDOW_BAR_HEIGHT, FILL_PARENT, FILL_PARENT - WINDOW_BAR_HEIGHT}});
        }));
        addChild(imageView);
        addChild(new WindowBar(
                context,
                Style().forEach([](StyleState& state) {
                    state.position = {0, 0, FILL_PARENT, WINDOW_BAR_HEIGHT};
                    state.background.color = rgba{COLOR_WINDOW_BAR_BG};
                }),
                imageView));
        addChild(
                (new DropDownView(
                        context,
                        Style({
                                      .position = {0, WINDOW_BAR_HEIGHT, 0, 0},
                                      .padding = padding(0)}),
                        {
                                DropDownViewElementPrototype("Fast exit", []() {
                                    throw std::exception();
                                }),
                                DropDownViewElementPrototype("Save to", [&]() { this->context->saveImage(); }),
                                DropDownViewElementPrototype("Open...", [&]() { this->context->openImage(); })
                        },
                        Style().forEach([](StyleState& state) {
                            state.position = {0, 0, FILL_PARENT, 24};
                            state.padding = padding(4);
                            state.fontRenderer = assets::fontRenderer("assets/fonts/segoe-ui/Segoe UI.ttf", 14);
                        }).edit([](Style& style) {
                            style.stateDefault.background.color = {COLOR_PRIMARY};
                            style.stateHover.background.color = {75, 110, 175, 255};
                            style.statePress.background.color = {66, 98, 155, 255};
                        })))->setId(APP_ICON_MENU_ID));
        auto* messageView = new MessageView(
                context,
                Style().forEach([](StyleState& state) {
                    state.position = {FILL_PARENT * 0.25, FILL_PARENT * 0.25, FILL_PARENT * 0.5, FILL_PARENT * 0.5};
                    state.padding = padding(12);
                    state.background.color = rgba{COLOR_PRIMARY_LIGHT};
                }));
        messageView->setId(MESSAGE_VIEW_ID);
        addChild(messageView);
    }
} // view