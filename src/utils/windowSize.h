#ifndef CPP_EXPERIMENTS_WINDOWSIZE_H
#define CPP_EXPERIMENTS_WINDOWSIZE_H

#include "GLFW/glfw3.h"

namespace utils {
    void checkWindowSize(GLFWwindow *window);
    int getWindowWidth();
    int getWindowHeight();
    void setOnWindowResize(void (*_onResize)(unsigned w, unsigned h));
}
#endif //CPP_EXPERIMENTS_WINDOWSIZE_H
