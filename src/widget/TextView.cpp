//
// Created by danil on 29.09.2022.
//

#include "TextView.h"

namespace view {
    TextView::TextView(Context* context, const Style& style) : View(context, style) {}

    TextView::TextView(Context* context, const Style& style, const String& _text) : View(context, style) {
        setText(_text);
    }

    void TextView::onDraw() {
//        styleState->fontRenderer->renderText(text, calculatedPos.x + styleState->padding.left,
//                                             calculatedPos.y + styleState->padding.top);
        styleState->fontRenderer->renderText(text, edges.left + styleState->padding.left + styleState->border.left,
                                             edges.top + styleState->padding.top + styleState->border.top);
    }

    void TextView::setText(const String& _text) {
        text = _text;
        invalidate();
    }

    const String& TextView::getText() const {
        return text;
    }

    View::SpaceRequirement TextView::onMeasure() {
        float width = styleState->padding.left + styleState->padding.right
                      + styleState->border.left + styleState->border.right
                      + styleState->width.pixel
                      + styleState->width.contentK * styleState->fontRenderer->getTextWidth(text);
        float height = styleState->padding.top + styleState->padding.bottom
                       + styleState->border.top + styleState->border.bottom
                       + styleState->height.pixel
                       + styleState->height.contentK * styleState->fontRenderer->getLineHeight();

        return {
                .width = width,
                .height = height,
                .parentPartW = styleState->width.parentK,
                .parentPartH = styleState->height.parentK,
                .parentSparePartW = styleState->width.parentSpareK,
                .parentSparePartH = styleState->height.parentSpareK};
    }

} // view