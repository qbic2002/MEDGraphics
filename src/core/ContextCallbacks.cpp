//
// Created by danil on 16.11.2022.
//

#include "ContextCallbacks.h"

void ContextCallbacks::onWindowResize(unsigned int width, unsigned int height) {}

void ContextCallbacks::onMouseMove(double x, double y) {}

bool ContextCallbacks::onDrag(double x, double y, double dx, double dy) {
    return false;
}

void ContextCallbacks::onWindowMaximize(int _maximized) {}

bool ContextCallbacks::onMouseButton(ClickEvent& event) {
    return false;
}

void ContextCallbacks::onMouseLeave() {}

void ContextCallbacks::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {}

void ContextCallbacks::onKey(int key, int scancode, int action, int mods) {}
