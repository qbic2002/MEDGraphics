//
// Created by danil on 23.09.2022.
//

#include "WindowBar.h"
#include "../view/ClickEvent.h"

namespace view {
    WindowBar::WindowBar(Context* context, const Style& style) : ViewGroup(context, style) {
        GLFWwindow* windowId = context->getWindowId();

        const unsigned iconSize = 16;
        const unsigned btnWidth = 46;
        const unsigned btnHeight = WINDOW_BAR_HEIGHT;

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
    }
} // view