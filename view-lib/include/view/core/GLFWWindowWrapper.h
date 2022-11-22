//
// Created by danil on 11.11.2022.
//

#ifndef MEDGRAPHICS_GLFW_WINDOW_WRAPPER_H
#define MEDGRAPHICS_GLFW_WINDOW_WRAPPER_H

#include "WindowWrapper.h"
#include "ContextCallbacks.h"
#include "GLFW/glfw3.h"
#include "utils/logging.h"

class GLFWWindowWrapper : public WindowWrapper {
public:
    GLFWWindowWrapper(ContextCallbacks* callbacks, GLFWwindow* window);

    void swapBuffers() override;

    bool shouldClose() override;

    void pollEvents() override;

    void waitEvents() override;

    /// @timeout time in seconds
    void waitEvents(double timeout) override;

    void setShouldClose(int value) override;

    void toggleMaximized() override;

    bool isMaximized() override;

    void iconify() override;

    void getWindowPos(int* x, int* y) override;

    void setWindowPos(int x, int y) override;

    void getWindowSize(int* width, int* height) override;

    void cursorPosCallback(double x, double y);

    void cursorEnterCallback(int entered);

    void keyCallback(int key, int scancode, int action, int mods);

    void mouseButtonCallback(int button, int action, int mods);

    void scrollCallback(double xOffset, double yOffset);

    void windowMaximizeCallback(int maximized);

    void windowRefreshCallback();

    void windowSizeCallback(int width, int height);

    void getPointerPos(double& x, double& y) override;

private:
    ContextCallbacks* callbacks;
    GLFWwindow* window;
    struct {
        double x = 0, y = 0;
    } cursorPos, grabPos;
    bool isGrabbing = false;
    struct {
        int width = 0;
        int height = 0;
    } windowSize;
    bool isMaximized_ = false;
};

#endif //MEDGRAPHICS_GLFW_WINDOW_WRAPPER_H
