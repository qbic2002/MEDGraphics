//
// Created by danil on 11.11.2022.
//

#include "view/core/GLFWWindowProvider.h"
#include <map>

std::map<GLFWwindow*, GLFWWindowWrapper*> wrappers;

void setCallbacks(GLFWwindow* window);

GLFWwindow* createWindow(uint width, uint height, const std::string& title, bool vsync);

GLFWWindowWrapper*
GLFWWindowProvider::createWindow(ContextCallbacks* callbacks, uint width, uint height, const std::string& title,
                                 bool vsync) {
    GLFWwindow* window = ::createWindow(width, height, title, vsync);

    auto* windowWrapper = new GLFWWindowWrapper(callbacks, window);
    setCallbacks(window);

    wrappers[window] = windowWrapper;
    return windowWrapper;
}

// Staff

void cursorPosCallback(GLFWwindow* window, double x, double y) {
    wrappers[window]->cursorPosCallback(x, y);
}

void cursorEnterCallback(GLFWwindow* window, int entered) {
    wrappers[window]->cursorEnterCallback(entered);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    wrappers[window]->mouseButtonCallback(button, action, mods);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    wrappers[window]->scrollCallback(xOffset, yOffset);
}

void windowMaximizeCallback(GLFWwindow* window, int maximized) {
    wrappers[window]->windowMaximizeCallback(maximized);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    wrappers[window]->keyCallback(key, scancode, action, mods);
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    wrappers[window]->windowSizeCallback(width, height);
}

void windowRefreshCallback(GLFWwindow* window) {
    wrappers[window]->windowRefreshCallback();
}

void setCallbacks(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetWindowMaximizeCallback(window, windowMaximizeCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetWindowRefreshCallback(window, windowRefreshCallback);
}

GLFWwindow* createWindow(uint width, uint height, const std::string& title, bool vsync) {
    GLFWwindow* window;

    if (!glfwInit())
        exceptional_exit("glfwInit() returned null");

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_VISIBLE, 0);

    window = glfwCreateWindow((int) width, (int) height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exceptional_exit("glfwCreateWindow() returned nullptr");
    }

    auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(vsync ? 1 : 0);

    glewInit();

    return window;
}
