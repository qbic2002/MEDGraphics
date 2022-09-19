#ifndef CPP_EXPERIMENTS_WINDOWSIZE_H
#define CPP_EXPERIMENTS_WINDOWSIZE_H

#include "GLFW/glfw3.h"

namespace utils {
    class OnWindowResizeListener {
    public:
        virtual void onWindowResize(unsigned int width, unsigned int height) = 0;
    };

    void checkWindowSize(GLFWwindow* window);

    int getWindowWidth();

    int getWindowHeight();

//    void setOnWindowResize(void (* _onResize)(unsigned w, unsigned h));
    void setOnWindowResize(OnWindowResizeListener* onWindowResizeListener);

    float getWindowRatio();
}
#endif //CPP_EXPERIMENTS_WINDOWSIZE_H
