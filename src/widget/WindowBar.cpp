//
// Created by danil on 23.09.2022.
//

#include "WindowBar.h"
#include "TextView.h"
#include "DropDownView.h"
#include <view/widget/LinearLayout.h>

namespace view {
    WindowBar::WindowBar(MyApp* context, const Style& style, ImageView* imageView)
            : LinearLayout(context, style, HORIZONTAL) {
        gravity = TOP;
        auto* windowWrapper = context->getWindowWrapper();

        /// Application Icon
        const unsigned appIconSize = 32;

        auto* view = new View(context, Style{}
                .forEach([context](StyleState& style) {
                    style.width = WINDOW_BAR_HEIGHT;
                    style.height = WINDOW_BAR_HEIGHT;
//                    style.position = {0, 0, WINDOW_BAR_HEIGHT, WINDOW_BAR_HEIGHT};
                    style.padding = padding((float) (WINDOW_BAR_HEIGHT - appIconSize) / 2);
                    style.background.setImage(context->getAppDir() / "assets/icons/ic_launcher.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
//        view->setOnClickListener([context]() {
//            auto* appMenu = (DropDownView*) context->findViewById(APP_ICON_MENU_ID);
//            appMenu->toggleOpened();
//        });
        addChild(view);

        /// Title Text View
        const unsigned titleBarFontSize = 14;
        auto* titleView = new TextView(
                context,
                Style{}
                        .forEach([context](StyleState& style) {
                            style.width = FILL_SPARE;
                            style.height = FILL_PARENT;
//                            state.position = {WINDOW_BAR_HEIGHT, 0, FILL_PARENT * 0.5, WINDOW_BAR_HEIGHT};
                            style.padding = padding((WINDOW_BAR_HEIGHT - (int) titleBarFontSize) / 2.0);
                            style.fontRenderer = assets::fontRenderer(
                                    context->getAppDir() / "assets/fonts/segoe-ui/Segoe UI.ttf",
                                    titleBarFontSize);
                        }),
                L"MED Graphics");
        addChild(titleView);

        context->getImageFileStorage().addImageTitleChangedListener([titleView, context]() {
            auto* imageFile = context->getImageFileStorage().getCurImageFile();
            if (imageFile == nullptr) {
                titleView->setText(L"MEDGraphics");
            } else {
                titleView->setText(imageFile->getPath().filename().wstring());
            }
        });

        const unsigned controlIconSize = 32;
        const unsigned controlSize = WINDOW_BAR_HEIGHT;
        const padding controlPadding = padding((float) (controlSize - controlIconSize) / 2);

        /// Fit Content Button
        view = new View(context, Style{}
                .forEach([controlPadding, context](StyleState& style) {
                    style.width = controlSize;
                    style.height = controlSize;
                    style.padding = controlPadding;
                    style.background.setImage(context->getAppDir() / "assets/icons/ic_fit_screen.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([imageView]() {
            imageView->imageFitScreen();
        });
        addChild(view);

        /// Original Scale Button
        view = new View(context, Style{}
                .forEach([controlPadding, context](StyleState& style) {
                    style.width = controlSize;
                    style.height = controlSize;
                    style.padding = controlPadding;
                    style.background.setImage(context->getAppDir() / "assets/icons/ic_original_scale.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([imageView]() {
            imageView->imageOriginalScale();
        });
        addChild(view);

        const unsigned iconSize = 16;
        const unsigned btnWidth = 46;
        const unsigned btnHeight = 30;

        const padding btnPadding = padding((float) (btnWidth - iconSize) / 2, (float) (btnHeight - iconSize) / 2);

        auto viewGroup = new LinearLayout(context, Style()
                                                  .forEach([](StyleState& style) {
                                                      style.width = btnWidth * 3;
                                                      style.height = btnHeight;
                                                  }),
                                          HORIZONTAL);
        addChild(viewGroup);

        /// Iconify Icon
        view = new View(context, Style()
                .forEach([context, btnPadding](StyleState& style) {
                    style.width = FILL_SPARE;
                    style.height = FILL_SPARE;
//                    style.position = {FILL_PARENT - btnWidth * 3, 0, btnWidth, btnHeight};
                    style.padding = btnPadding;
                    style.background.setImage(context->getAppDir() / "assets/icons/ic_iconify.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([windowWrapper]() {
            windowWrapper->iconify();
        });
        viewGroup->addChild(view);

        /// Minimize-Maximize Icon
        view = new View(context, Style()
                .forEach([btnPadding](StyleState& style) {
                    style.width = FILL_SPARE;
                    style.height = FILL_SPARE;
//                    style.position = {FILL_PARENT - btnWidth * 2, 0, btnWidth, btnHeight};
                    style.padding = btnPadding;
                    style.background.setImage("assets/icons/ic_minimized.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([windowWrapper]() {
            windowWrapper->toggleMaximized();
        });
        view->setOnWindowResizeListener([context, windowWrapper](View& view, unsigned width, unsigned height) {
            view.getStyle().forEach([context, windowWrapper](StyleState& state) {
                state.background.setImage(windowWrapper->isMaximized()
                                          ? context->getAppDir() / "assets/icons/ic_maximized.png"
                                          : context->getAppDir() / "assets/icons/ic_minimized.png");
            });
        });
        viewGroup->addChild(view);

        /// Close Icon
        view = new View(context, Style()
                .forEach([&btnPadding, context](StyleState& style) {
                    style.width = FILL_SPARE;
                    style.height = FILL_SPARE;
//                    style.position = {FILL_PARENT - btnWidth, 0, btnWidth, btnHeight};
                    style.padding = btnPadding;
                    style.background.setImage(context->getAppDir() / "assets/icons/ic_close.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {196, 43, 28, 255};
                    style.statePress.background.color = {198, 99, 99, 255};
                }));
        view->setOnClickListener([windowWrapper]() {
            windowWrapper->setShouldClose(1);
        });
        viewGroup->addChild(view);
    }
} // view