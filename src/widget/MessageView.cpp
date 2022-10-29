//
// Created by danil on 14.10.2022.
//

#include "MessageView.h"

view::MessageView::MessageView(view::Context* context, const view::Style& style) : ViewGroup(context, style) {
    addChild(new TextView(
            context,
            Style().forEach([context](StyleState& state) {
                state.position = {0, 0, FILL_PARENT, 30};
                state.padding = padding(12);
                state.fontRenderer = assets::fontRenderer(
                        context->getAppDir() / "assets/fonts/segoe-ui/Segoe UI.ttf", 18);
            }),
            title));

    const unsigned iconSize = 16;
    const unsigned btnWidth = 46;
    const unsigned btnHeight = 30;

    const padding btnPadding = padding((float) (btnWidth - iconSize) / 2, (float) (btnHeight - iconSize) / 2);

    /// Close Icon
    auto* closeBtn = new View(context, Style()
            .forEach([&btnPadding](StyleState& style) {
                style.position = {FILL_PARENT - btnWidth, 0, btnWidth, btnHeight};
                style.padding = btnPadding;
                style.background.setImage("assets/icons/ic_close.png");
            })
            .edit([](Style& style) {
                style.stateHover.background.color = {COLOR_PRIMARY};
                style.statePress.background.color = {COLOR_PRIMARY_DARK};
            }));
    closeBtn->setOnClickListener([&]() {
        this->close();
    });
    addChild(closeBtn);

    msgView = new TextView(
            context,
            Style().forEach([context](StyleState& state) {
                state.position = {0, 30, FILL_PARENT, FILL_PARENT - 30};
                state.padding = padding(12);
                state.fontRenderer = assets::fontRenderer(
                        context->getAppDir() / "assets/fonts/segoe-ui/Segoe UI.ttf", 14);
            }),
            L"ErrorText");
    addChild(msgView);
}

void view::MessageView::showMessage(const String& _msg) {
    msgView->setText(_msg);
    isOpened = true;
}

void view::MessageView::close() {
    isOpened = false;
}

void view::MessageView::render() {
    if (!isOpened) return;
    ViewGroup::render();
}
