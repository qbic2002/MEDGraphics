//
// Created by danil on 18.11.2022.
//

#include "ExpApp.h"
#include "../widget/TextView.h"
#include "view/widget/LinearLayout.h"

void ExpApp::onCreated(const std::vector<std::wstring>& args) {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    using namespace view;

    auto* lay = new LinearLayout(
            this,
            Style{{
                          .width = FILL_PARENT,
                          .height = FILL_PARENT,
                          .background = {127, 29, 38, 255}}},
            view::HORIZONTAL);

    auto* textView = new TextView(
            this,
            Style{{
                          .width = WRAP_CONTENT,
                          .height = WRAP_CONTENT,
                          .padding = padding(8),
                          .background = {29, 127, 38, 255},
                          .fontRenderer = assets::fontRenderer("assets/fonts/MinecraftRegular.otf", 18)}},
            L"Hello world");
    lay->addChild(textView);

    auto* textView2 = new TextView(
            this,
            Style{{
                          .width = FILL_PARENT / 2,
                          .height = WRAP_CONTENT,
                          .padding = padding(16),
                          .background = {38, 29, 127, 255},
                          .fontRenderer = assets::fontRenderer("assets/fonts/MinecraftRegular.otf", 18)
                  }}.edit([](Style& style) {
                style.stateHover.background = {127, 29, 127, 255};
                style.statePress.background = {100, 20, 100, 255};
            }), L"Line 2");
    lay->addChild(textView2);

    setRootView(lay);
}
