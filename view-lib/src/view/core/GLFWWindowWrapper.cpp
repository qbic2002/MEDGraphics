//
// Created by danil on 11.11.2022.
//

#include "view/core/GLFWWindowWrapper.h"

void GLFWWindowWrapper::swapBuffers() {
    glfwSwapBuffers(window);
}

bool GLFWWindowWrapper::shouldClose() {
    return glfwWindowShouldClose(window);
}

void GLFWWindowWrapper::pollEvents() {
    glfwPollEvents();
}

void GLFWWindowWrapper::setShouldClose(int value) {
    glfwSetWindowShouldClose(window, value);
}

void GLFWWindowWrapper::toggleMaximized() {
    (isMaximized_ ? glfwRestoreWindow : glfwMaximizeWindow)(window);
}

bool GLFWWindowWrapper::isMaximized() {
    return isMaximized_;
}

void GLFWWindowWrapper::iconify() {
    glfwIconifyWindow(window);
}

void GLFWWindowWrapper::getWindowPos(int* x, int* y) {
    glfwGetWindowPos(window, x, y);
}

void GLFWWindowWrapper::setWindowPos(int x, int y) {
    glfwSetWindowPos(window, x, y);
}

void GLFWWindowWrapper::getWindowSize(int* width, int* height) {
    *width = windowSize.width;
    *height = windowSize.height;
}

/* ======================================== *
 * =              Callbacks               = *
 * ======================================== */

void GLFWWindowWrapper::cursorEnterCallback(int entered) {
    if (entered == 0)
        callbacks->onMouseLeave();
}

void GLFWWindowWrapper::cursorPosCallback(double x, double y) {
    auto dx = x - cursorPos.x;
    auto dy = y - cursorPos.y;
    cursorPos = {x, y};
    callbacks->onMouseMove(x, y);

    if (isGrabbing) {
        if (callbacks->onDrag(x, y, dx, dy)) {
            grabPos = cursorPos;
            return;
        }
        if (!isMaximized_) {
            int windowX, windowY;
            getWindowPos(&windowX, &windowY);
            setWindowPos(windowX + x - grabPos.x, windowY + y - grabPos.y);
        }
    }
}

void GLFWWindowWrapper::keyCallback(int key, int scancode, int action, int mods) {
    callbacks->onKey(key, scancode, action, mods);
}

void GLFWWindowWrapper::mouseButtonCallback(int button, int action, int mods) {
    ClickEvent event{cursorPos.x, cursorPos.y, button, action, mods};

    if (event.action == GLFW_RELEASE && isGrabbing)
        isGrabbing = false;

    if (callbacks->onMouseButton(event))
        return;

    if (action == GLFW_PRESS && !isGrabbing) {
        isGrabbing = true;
        grabPos = cursorPos;
    }
}

void GLFWWindowWrapper::scrollCallback(double xOffset, double yOffset) {
    callbacks->onScroll(xOffset, yOffset, cursorPos.x, cursorPos.y);
}

void GLFWWindowWrapper::windowMaximizeCallback(int maximized) {
    callbacks->onWindowMaximize(maximized);
    isMaximized_ = maximized;
}

void GLFWWindowWrapper::windowRefreshCallback() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    windowSizeCallback(width, height);
}

void GLFWWindowWrapper::windowSizeCallback(int width, int height) {
    if (windowSize.width == width && windowSize.height == height)
        return;
    windowSize.width = width;
    windowSize.height = height;
    callbacks->onWindowResize(width, height);
}
