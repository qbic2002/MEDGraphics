#include <iostream>
#include "windowSize.h"

namespace utils {
    int curWidth = -1;
    int curHeight = -1;

    void (* onResize)(unsigned w, unsigned h);

    OnWindowResizeListener* onWindowResizeListener = nullptr;

    void checkWindowSize(GLFWwindow* window) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (width != curWidth || height != curHeight) {
            glViewport(0, 0, width, height);
            curWidth = width;
            curHeight = height;
            std::cout << "Resized to: " << width << "x" << height << std::endl;
            if (onWindowResizeListener)
                onWindowResizeListener->onWindowResize(width, height);
        }
    }

    int getWindowWidth() {
        return curWidth;
    }

    int getWindowHeight() {
        return curHeight;
    }

    void setOnWindowResize(void (* _onResize)(unsigned w, unsigned h)) {
        onResize = _onResize;
    }

    float getWindowRatio() {
        return (float) curWidth / curHeight;
    }

    void setOnWindowResize(OnWindowResizeListener* _onWindowResizeListener) {
        onWindowResizeListener = _onWindowResizeListener;
    }
}