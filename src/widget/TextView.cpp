//
// Created by danil on 29.09.2022.
//

#include "TextView.h"
#include "utils/gl_utils.h"

namespace view {
    TextView::TextView(Context* context, const TextViewAttributes& attr) : View(context, (const ViewAttributes&) attr) {
        TEXT_VIEW_ATTRS_SET(attr)
        if (font.string().empty()) {
            font = context->getAppDir() / "assets/fonts/segoe-ui/Segoe UI.ttf";
        }
        setFontSize(attr.fontSize);
    }

    void TextView::onDraw() {
        glColor(fontColor);
        fontRenderer->renderText(text, innerEdges.left, innerEdges.top);
    }

    float TextView::getContentWidth() {
        return fontRenderer->getTextWidth(text);
    }

    float TextView::getContentHeight() {
        return fontRenderer->getLineHeight();
    }

    void TextView::setText(const String& _text) {
        text = _text;
        invalidate();
    }

    const String& TextView::getText() {
        return text;
    }

    void TextView::setFontSize(int fontSize) {
        fontRenderer = assets::fontRenderer(font, fontSize);
        invalidate();
    }

} // view