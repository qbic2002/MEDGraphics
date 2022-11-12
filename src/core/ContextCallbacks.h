//
// Created by danil on 11.11.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_CALLBACKS_H
#define MEDGRAPHICS_CONTEXT_CALLBACKS_H

#include "../view/ClickEvent.h"

class ContextCallbacks {
public:
    /// Window resize event interface
    /// @param width new window width
    /// @param height new window height
    virtual void onWindowResize(unsigned int width, unsigned int height) {};

    /// Pointer move event interface
    /// @param x new pointer x - relative
    /// @param y new pointer y - relative
    virtual void onMouseMove(double x, double y) {};

    /// Window maximize/minimize event interface
    /// @param _maximized 0 - window was minimized, 1 - window was maximized
    virtual void onWindowMaximize(int _maximized) {};

    /// Mouse button event interface
    /// @param event event to process
    virtual void onMouseButton(ClickEvent& event) {};

    /// Pointer window leave event interface
    virtual void onMouseLeave() {};

    /// Mouse scroll event interface
    /// @param xOffset scroll x distance
    /// @param yOffset scroll y distance
    /// @param cursorX window relative pointer x
    /// @param cursorY window relative pointer y
    virtual void onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {};

    /// Keyboard input event interface
    /// @param window The window that received the event.
    /// @param key The key that was pressed or released.
    /// @param scancode The system-specific scancode of the key.
    /// @param action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`
    virtual void onKey(int key, int scancode, int action, int mods) {};
};

#endif //MEDGRAPHICS_CONTEXT_CALLBACKS_H
