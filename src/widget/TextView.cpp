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
        styleState->fontRenderer->renderText(text, calculatedPos.x + styleState->padding.left,
                                             calculatedPos.y + styleState->padding.top);
    }

    void TextView::setText(const std::string& _text) {
        text = _text;
    }

    const std::string& TextView::getText() const {
        return text;
    }
} // view