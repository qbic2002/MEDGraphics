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
                          .background = {127, 29, 38, 255},
                          .width = WRAP_CONTENT,
                          .height = FILL_PARENT}},
            view::HORIZONTAL);

    auto* textView = new TextView(
            this,
            Style{{
                          .padding = padding(8),
                          .background = {29, 127, 38, 255},
                          .fontRenderer = assets::fontRenderer("assets/fonts/MinecraftRegular.otf", 18),
                          .width = WRAP_CONTENT,
                          .height = WRAP_CONTENT}},
            L"Hello world");
    lay->addChild(textView);

    auto* textView2 = new TextView(
            this,
            Style{{
                          .background = {38, 29, 127, 255},
                          .fontRenderer = assets::fontRenderer("assets/fonts/MinecraftRegular.otf", 18),
                          .width = WRAP_CONTENT,
                          .height = WRAP_CONTENT,
                  }}, L"Line 2");
    lay->addChild(textView2);

    setRootView(lay);
}
