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

    auto* lay = new LinearLayout(this, {
            .width = FILL_PARENT,
            .height = FILL_PARENT,
            .background = ColorBackground{rgba{127, 29, 38, 255}},
            .orientation = VERTICAL,
            .gravity = RIGHT
    });

    auto* textView = new TextView(this, {
            .width = WRAP_CONTENT,
            .height = WRAP_CONTENT,
            .padding = Padding(8),
            .background = ColorBackground{rgba{29, 127, 38, 255}},
            .text = L"Hello world",
            .font = "assets/fonts/MinecraftRegular.otf",
            .fontSize = 18
    });
    lay->addChild(textView);

    auto* textView2 = new TextView(this, {
            .width = FILL_PARENT / 2,
            .height = WRAP_CONTENT,
            .padding = Padding(16),
            .background = StateBackground{
                    ColorBackground{rgba{38, 29, 127, 255}},
                    ColorBackground{rgba{127, 29, 127, 255}},
                    ColorBackground{rgba{100, 20, 100, 255}}},
            .text = L"Line 2",
            .font = "assets/fonts/MinecraftRegular.otf",
            .fontSize = 18
    });
    lay->addChild(textView2);

    setRootView(lay);
}
