//
// Created by danil on 11.11.2022.
//

#include "GLFWWindowWrapper.h"

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

void GLFWWindowWrapper::cursorPosCallback(double x, double y) {
    cursorPos.x = x;
    cursorPos.y = y;
    callbacks->onMouseMove(x, y);
}

void GLFWWindowWrapper::cursorEnterCallback(int entered) {
    if (entered == 0)
        callbacks->onMouseLeave();
}

void GLFWWindowWrapper::mouseButtonCallback(int button, int action, int mods) {
    ClickEvent event{cursorPos.x, cursorPos.y, button, action, mods};
    callbacks->onMouseButton(event);
}

void GLFWWindowWrapper::windowMaximizeCallback(int maximized) {
    callbacks->onWindowMaximize(maximized);
    isMaximized_ = maximized;
}

void GLFWWindowWrapper::scrollCallback(double xOffset, double yOffset) {
    callbacks->onScroll(xOffset, yOffset, cursorPos.x, cursorPos.y);
}

void GLFWWindowWrapper::keyCallback(int key, int scancode, int action, int mods) {
    callbacks->onKey(key, scancode, action, mods);
}

void GLFWWindowWrapper::windowSizeCallback(int width, int height) {
    if (windowSize.width == width && windowSize.height == height)
        return;
    windowSize.width = width;
    windowSize.height = height;
    callbacks->onWindowResize(width, height);
}

void GLFWWindowWrapper::windowRefreshCallback() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    windowSizeCallback(width, height);
}

void GLFWWindowWrapper::getWindowSize(int* width, int* height) {
    *width = windowSize.width;
    *height = windowSize.height;
}
