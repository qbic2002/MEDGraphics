//
// Created by danil on 23.09.2022.
//

#include "ImageView.h"
#include "WindowBar.h"
#include "TextView.h"
#include "DropDownView.h"
#include <filesystem>

namespace view {
    WindowBar::WindowBar(Context* context, const Style& style, ImageView* imageView) : ViewGroup(context, style) {
        GLFWwindow* windowId = context->getWindowId();

        const unsigned iconSize = 16;
        const unsigned btnWidth = 46;
        const unsigned btnHeight = 30;

        const padding btnPadding = padding((float) (btnWidth - iconSize) / 2, (float) (btnHeight - iconSize) / 2);

        /// Close Icon
        auto view = new View(context, Style()
                .forEach([&btnPadding](StyleState& style) {
                    style.position = {FILL_PARENT - btnWidth, 0, btnWidth, btnHeight};
                    style.padding = btnPadding;
                    style.background.setImage("assets/icons/ic_close.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {196, 43, 28, 255};
                    style.statePress.background.color = {198, 99, 99, 255};
                }));
        view->setOnClickListener([windowId]() {
            glfwSetWindowShouldClose(windowId, 1);
        });
        addChild(view);

        /// Minimize-Maximize Icon
        view = new View(context, Style()
                .forEach([btnPadding](StyleState& style) {
                    style.position = {FILL_PARENT - btnWidth * 2, 0, btnWidth, btnHeight};
                    style.padding = btnPadding;
                    style.background.setImage("assets/icons/ic_minimized.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([context, windowId]() {
            (context->isMaximized() ? glfwRestoreWindow : glfwMaximizeWindow)(windowId);
        });
        view->setOnWindowResizeListener([](View& view, unsigned width, unsigned height) {
            view.getStyle().forEach([&view](StyleState& state) {
                state.background.setImage(view.getContext()->isMaximized()
                                          ? view.getContext()->getRootDirectory() + "assets/icons/ic_maximized.png"
                                          : view.getContext()->getRootDirectory() + "assets/icons/ic_minimized.png");
            });
        });
        addChild(view);

        /// Iconify Icon
        view = new View(context, Style()
                .forEach([btnPadding](StyleState& style) {
                    style.position = {FILL_PARENT - btnWidth * 3, 0, btnWidth, btnHeight};
                    style.padding = btnPadding;
                    style.background.setImage("assets/icons/ic_iconify.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([windowId]() {
            glfwIconifyWindow(windowId);
        });
        addChild(view);

        const unsigned controlIconSize = 32;
        const unsigned controlSize = WINDOW_BAR_HEIGHT;

        const padding controlPadding = padding((float) (controlSize - controlIconSize) / 2);

        /// Fit Content Button
        view = new View(context, Style{}
                .forEach([controlPadding](StyleState& style) {
                    style.position = {FILL_PARENT - btnWidth * 3 - controlSize - 12, 0, controlSize, controlSize};
                    style.padding = controlPadding;
                    style.background.setImage("assets/icons/ic_fit_screen.png");
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
                .forEach([controlPadding](StyleState& style) {
                    style.position = {FILL_PARENT - btnWidth * 3 - controlSize * 2 - 12, 0, controlSize, controlSize};
                    style.padding = controlPadding;
                    style.background.setImage("assets/icons/ic_original_scale.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([imageView]() {
            imageView->imageOriginalScale();
        });
        addChild(view);

        /// Application Icon
        const unsigned appIconSize = 32;

        view = new View(context, Style{}
                .forEach([](StyleState& style) {
                    style.position = {0, 0, WINDOW_BAR_HEIGHT, WINDOW_BAR_HEIGHT};
                    style.padding = padding((float) (WINDOW_BAR_HEIGHT - appIconSize) / 2);
                    style.background.setImage("assets/icons/ic_launcher.png");
                })
                .edit([](Style& style) {
                    style.stateHover.background.color = {COLOR_WINDOW_BAR_BG_HOVER};
                    style.statePress.background.color = {COLOR_WINDOW_BAR_BG_PRESS};
                }));
        view->setOnClickListener([context]() {
            auto* appMenu = (DropDownView*) context->getRootView()->findViewById(APP_ICON_MENU_ID);
            appMenu->toggleOpened();
        });
        view->setOnClickListener([context]() {
            auto* appMenu = (DropDownView*) context->getRootView()->findViewById(APP_ICON_MENU_ID);
            appMenu->toggleOpened();
        });
        addChild(view);

        /// Title Text View
        const unsigned titleBarFontSize = 14;
        auto* titleView = new TextView(
                context,
                Style{}
                        .forEach([](StyleState& state) {
                            state.position = {WINDOW_BAR_HEIGHT, 0, FILL_PARENT * 0.5, WINDOW_BAR_HEIGHT};
                            state.padding = padding((WINDOW_BAR_HEIGHT - (int) titleBarFontSize) / 2.0);
                            state.fontRenderer = assets::fontRenderer("assets/fonts/segoe-ui/Segoe UI.ttf",
                                                                      titleBarFontSize);
                        }),
                "MED Graphics");
        addChild(titleView);

        context->addImageChangedListener([titleView, context]() {
            titleView->setText(std::filesystem::path(context->getCurrentImageData().fileName).filename().string());
        });
    }
} // view