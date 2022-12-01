//
// Created by danil on 01.12.2022.
//

#include "EditText.h"

namespace view {
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

    bool EditText::onClick(const ClickEvent& event) {
        if (event.action == GLFW_PRESS)
            return false;

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
                    setCursorIndex(cursorIndex - 1);
                    invalidate();
                }
                return true;
            case GLFW_KEY_DELETE:
                if (text.length() > 0 && cursorIndex < text.length()) {
                    text.erase(cursorIndex, 1);
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
        invalidate();
        setCursorIndex(cursorIndex + 1);

        return false;
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
} // view