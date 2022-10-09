//
// Created by danil on 23.09.2022.
//

#include "ImageView.h"
#include "WindowBar.h"
#include "TextView.h"
#include <filesystem>

namespace view {
    WindowBar::WindowBar(Context* context, const Style& style, ImageView* imageView) : ViewGroup(context, style) {
        GLFWwindow* windowId = context->getWindowId();

        const unsigned iconSize = 16;
        const unsigned btnWidth = 46;
        const unsigned btnHeight = 30;

        const padding btnPadding = padding((float) (btnWidth - iconSize) / 2, (float) (btnHeight - iconSize) / 2);

        /// Close Icon
        auto view = new View(context, Style{
                .position = {FILL_PARENT - btnWidth, 0, btnWidth, btnHeight},
                .padding = btnPadding,
                .background = Background{
                        .colorOnHover = {196, 43, 28, 255},
                        .colorOnPress = {198, 99, 99, 255}}
                        .setImage("assets/icons/ic_close.png")});
        view->setOnClickListener([windowId]() {
            glfwSetWindowShouldClose(windowId, 1);
        });
        addChild(view);

        /// Minimize-Maximize Icon
        view = new View(context, Style{
                .position = {FILL_PARENT - btnWidth * 2, 0, btnWidth, btnHeight},
                .padding = btnPadding,
                .background = Background{
                        .colorOnHover = {127, 127, 127, 127},
                        .colorOnPress = {63, 63, 63, 127}}
                        .setImage("assets/icons/ic_minimized.png")});
        view->setOnClickListener([context, windowId]() {
            (context->isMaximized() ? glfwRestoreWindow : glfwMaximizeWindow)(windowId);
        });
        view->setOnWindowResizeListener([](View& view, unsigned width, unsigned height) {
            view.getStyle().background.setImage(view.getContext()->isMaximized()
                                                ? "assets/icons/ic_maximized.png"
                                                : "assets/icons/ic_minimized.png");
        });
        addChild(view);

        /// Iconify Icon
        view = new View(context, Style{
                .position = {FILL_PARENT - btnWidth * 3, 0, btnWidth, btnHeight},
                .padding = btnPadding,
                .background = Background{
                        .colorOnHover = {127, 127, 127, 127},
                        .colorOnPress = {63, 63, 63, 127}}
                        .setImage("assets/icons/ic_iconify.png")});
        view->setOnClickListener([windowId]() {
            glfwIconifyWindow(windowId);
        });
        addChild(view);

        const unsigned controlIconSize = 32;
        const unsigned controlSize = WINDOW_BAR_HEIGHT;

        const padding controlPadding = padding((float) (controlSize - controlIconSize) / 2);

        /// Fit Content Button
        view = new View(context, Style{
                .position = {FILL_PARENT - btnWidth * 3 - controlSize - 12, 0, controlSize, controlSize},
                .padding = controlPadding,
                .background = Background{
                        .colorOnHover = {127, 127, 127, 127},
                        .colorOnPress = {63, 63, 63, 127}}
                        .setImage("assets/icons/ic_fit_screen.png")});
        view->setOnClickListener([imageView]() {
            imageView->imageFitScreen();
        });
        addChild(view);

        /// Original Scale Button
        view = new View(context, Style{
                .position = {FILL_PARENT - btnWidth * 3 - controlSize * 2 - 12, 0, controlSize, controlSize},
                .padding = controlPadding,
                .background = Background{
                        .colorOnHover = {127, 127, 127, 127},
                        .colorOnPress = {63, 63, 63, 127}}
                        .setImage("assets/icons/ic_original_scale.png")});
        view->setOnClickListener([imageView]() {
            imageView->imageOriginalScale();
        });
        addChild(view);

        /// Application Icon
        const unsigned appIconSize = 32;

        view = new View(context, Style{
                .position = {0, 0, WINDOW_BAR_HEIGHT, WINDOW_BAR_HEIGHT},
                .padding = padding((float) (WINDOW_BAR_HEIGHT - appIconSize) / 2),
                .background = Background{
                        .colorOnHover = {127, 127, 127, 127},
                        .colorOnPress = {63, 63, 63, 127}}
                        .setImage("assets/icons/ic_launcher.png")});
        addChild(view);

        /// Title Text View
        unsigned titleBarFontSize = 14;
        auto* titleView = new TextView(context, Style{
                .position = {WINDOW_BAR_HEIGHT, 0, FILL_PARENT * 0.5, WINDOW_BAR_HEIGHT},
                .padding = padding((WINDOW_BAR_HEIGHT - (int) titleBarFontSize) / 2.0),
                .fontRenderer = assets::fontRenderer("assets/fonts/segoe-ui/Segoe UI.ttf", titleBarFontSize)
        }, "Hello world! test underline chars q g p , j y");
        addChild(titleView);

        context->addImageChangedListener([titleView, context]() {
            titleView->setText(std::filesystem::path(context->getCurrentImageData().fileName).filename().string());
        });
    }
} // view