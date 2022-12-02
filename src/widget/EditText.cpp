//
// Created by danil on 01.12.2022.
//

#include "EditText.h"

namespace view {
    EditText::EditText(Context* context, const EditTextAttributes& attr)
            : TextView(context, *((const TextViewAttributes*) &attr)) {
        EDIT_TEXT_VIEW_ATTRS_SET(attr)
        validateText();
    }

    void EditText::onDraw() {
        TextView::onDraw();

        if (isFocused) {
            float lineShift = fontRenderer->getTextWidth(text, 0, cursorIndex);

            glLineWidth(0.5);
            glBegin(GL_LINES);
            {
                glVertex2f(innerEdges.left + lineShift + 0.5, innerEdges.top);
                glVertex2f(innerEdges.left + lineShift + 0.5, innerEdges.bottom);
            }
            glEnd();
        }
    }

    bool EditText::onClick(const MouseEvent& event) {
        setFocused(true);
        int index = fontRenderer->positionToIndex(text, event.x - innerEdges.left, event.y - innerEdges.top);
        setCursorIndex(index);

        return true;
    }

    bool EditText::onKey(int key, int scancode, int action, int mods) {
        if (!isFocused)
            return false;
        if (action == GLFW_PRESS)
            return false;

        switch (key) {
            case GLFW_KEY_ESCAPE:
                setFocused(false);
                return true;
            case GLFW_KEY_RIGHT:
                setCursorIndex(cursorIndex + 1);
                return true;
            case GLFW_KEY_LEFT:
                setCursorIndex(cursorIndex - 1);
                return true;
            case GLFW_KEY_HOME:
                setCursorIndex(0);
                return true;
            case GLFW_KEY_END:
                setCursorIndex(text.length());
                return true;
            case GLFW_KEY_BACKSPACE:
                if (text.length() > 0 && cursorIndex > 0) {
                    text.erase(cursorIndex - 1, 1);
                    validateText();
                    setCursorIndex(cursorIndex - 1);
                    invalidate();
                }
                return true;
            case GLFW_KEY_DELETE:
                if (text.length() > 0 && cursorIndex < text.length()) {
                    text.erase(cursorIndex, 1);
                    validateText();
                    invalidate();
                }
                return true;

        }

        return false;
    }

    bool EditText::onChar(unsigned int codepoint) {
        if (!isFocused)
            return false;

        wchar_t charToInsert[2] = {(wchar_t) codepoint, 0};
        text.insert(cursorIndex, charToInsert);
        validateText();
        invalidate();
        setCursorIndex(cursorIndex + 1);

        return true;
    }

    void EditText::setFocused(bool value) {
        if (isFocused == value)
            return;
        isFocused = value;
        invalidate();
        if (!isFocused)
            setCursorIndex(0);
    }

    void EditText::setCursorIndex(int index) {
        if (index < 0)
            index = 0;
        if (index > text.length())
            index = text.length();
        if (cursorIndex == index)
            return;
        cursorIndex = index;
        invalidate();
    }

    void EditText::validateText() {
        size_t textLength = text.length();
        auto iter = text.begin();
        switch (inputType) {
            case TEXT:
                break;
            case INTEGER: {
                bool canHaveMinus = true;
                while (iter != text.end()) {
                    auto c = *iter;
                    if (('0' <= c && c <= '9') || (canHaveMinus && c == '-')) {
                        canHaveMinus = false;
                        iter++;
                    } else {
                        iter = text.erase(iter);
                    }
                }
                break;
            }
            case DECIMAL: {
                bool canHaveMinus = true;
                bool canHavePoint = true;
                while (iter != text.end()) {
                    auto c = *iter;
                    if (('0' <= c && c <= '9') || (canHaveMinus && c == '-')) {
                        canHaveMinus = false;
                        iter++;
                    } else if (canHavePoint && c == '.') {
                        canHaveMinus = false;
                        canHavePoint = false;
                        iter++;
                    } else {
                        iter = text.erase(iter);
                    }
                }
                break;
            }
        }
        if (textLength != text.length()) { // text changed
            invalidate();
        }
    }

    void EditText::setText(const String& text) {
        TextView::setText(text);
        validateText();
    }
} // view