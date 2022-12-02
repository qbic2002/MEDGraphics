//
// Created by danil on 16.11.2022.
//

#include "view/core/ContextCallbacks.h"

void ContextCallbacks::onWindowResize(unsigned int width, unsigned int height) {}

void ContextCallbacks::onMouseMove(double x, double y) {}

bool ContextCallbacks::onDrag(double x, double y, double dx, double dy) {
    return false;
}

void ContextCallbacks::onWindowMaximize(int _maximized) {}

bool ContextCallbacks::onMouseButton(MouseEvent& event) {
    return false;
}

void ContextCallbacks::onMouseLeave() {}

void ContextCallbacks::onMouseEnter(double x, double y) {}

void ContextCallbacks::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {}

bool ContextCallbacks::onKey(int key, int scancode, int action, int mods) {
    return false;
}

bool ContextCallbacks::onChar(unsigned int codepoint) {
    return false;
}
