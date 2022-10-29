//
// Created by danil on 29.09.2022.
//

#include "TextView.h"

namespace view {
    TextView::TextView(Context* context, const Style& style) : View(context, style) {}

    TextView::TextView(Context* context, const Style& style, const String& _text) : View(context, style) {
        setText(_text);
    }

    void TextView::render() {
        View::renderBackground();
        styleState->fontRenderer->renderText(text, calculatedPos.x + styleState->padding.left,
                                             calculatedPos.y + styleState->padding.top);
    }

    void TextView::setText(const String& _text) {
        text = _text;
    }

    const String& TextView::getText() const {
        return text;
    }
} // view