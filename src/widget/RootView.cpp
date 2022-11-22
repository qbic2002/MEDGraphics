//
// Created by danil on 23.09.2022.
//

#include "RootView.h"
#include "BgRenderer.h"
#include "ImageView.h"
#include "WindowBar.h"
#include "DropDownView.h"

namespace view {
    RootView::RootView(MyApp* context, const Style& style) : LinearLayout(context, style) {
//        addChild(new BgRenderer(context));
        auto* imageView = new ImageView(context, Style({StyleState{.width = FILL_PARENT, .height = FILL_SPARE}})
                .edit([](Style& style) {
                    style.isDraggable = true;
                }));
        addChild(new WindowBar(context,
                               StyleState{
                                       .width = FILL_PARENT,
                                       .height = WINDOW_BAR_HEIGHT,
                                       .background = {rgba{COLOR_WINDOW_BAR_BG}}},
                               imageView));
        addChild(imageView);
//        addChild(
//                (new DropDownView(
//                        context,
//                        Style({
//                                      .position = {0, WINDOW_BAR_HEIGHT, 0, 0},
//                                      .padding = padding(0)}),
//                        {
//                                DropDownViewElementPrototype(L"Fast exit", []() {
//                                    throw std::exception();
//                                }),
//                                DropDownViewElementPrototype(L"Save to", [context]() { context->saveImage(); }),
//                                DropDownViewElementPrototype(L"Open...", [context]() { context->openImage(); })
//                        },
//                        Style().forEach([context](StyleState& state) {
//                            state.position = {0, 0, FILL_PARENT, 24};
//                            state.padding = padding(4);
//                            state.fontRenderer = assets::fontRenderer(
//                                    context->getAppDir() / "assets/fonts/segoe-ui/Segoe UI.ttf", 14);
//                        }).edit([](Style& style) {
//                            style.stateDefault.background.color = {COLOR_PRIMARY};
//                            style.stateHover.background.color = {75, 110, 175, 255};
//                            style.statePress.background.color = {66, 98, 155, 255};
//                        })))->setId(APP_ICON_MENU_ID));
//        auto* messageView = new MessageView(
//                context,
//                Style().forEach([](StyleState& state) {
//                    state.position = {FILL_PARENT * 0.25, FILL_PARENT * 0.25, FILL_PARENT * 0.5, FILL_PARENT * 0.5};
//                    state.padding = padding(12);
//                    state.background.color = rgba{COLOR_PRIMARY_LIGHT};
//                }));
//        messageView->setId(MESSAGE_VIEW_ID);
//        addChild(messageView);

    }
} // view