//
// Created by danil on 29.09.2022.
//

#include "TextView.h"

namespace view {
    TextView::TextView(Context* context, const Style& style) : View(context, style) {}

    TextView::TextView(Context* context, const Style& style, const std::string& _text) : View(context, style),
                                                                                         text(_text) {}

    void TextView::render() {
        View::renderBackground();
        style.fontRenderer->renderText(text, calculatedPos.x + style.padding.left, calculatedPos.y + style.padding.top);
    }

    void TextView::setText(const std::string& _text) {
        text = _text;
    }
} // view