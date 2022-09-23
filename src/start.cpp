#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "utils/measureFps.h"
#include "utils/windowSize.h"
#include "pnm/PNMImage.h"
#include "gl/ShaderProgram.h"
#include "view/Context.h"

using namespace std;
using namespace utils;
using namespace view;

Context* context;

GLFWwindow* createWindow(int width, int height, const char* title, bool vsync) {
    GLFWwindow* window;

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_DECORATED, 0);
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(vsync ? 1 : 0);
    return window;
}

struct {
    double x, y;
} cursorPos;

void cursorPosCallback(GLFWwindow* window, double x, double y) {
    cursorPos.x = x;
    cursorPos.y = y;
    context->onMouseMove(x, y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    ClickEvent event{cursorPos.x, cursorPos.y, button, action, mods};
    context->onMouseButton(event);
}

void cursorEnterCallback(GLFWwindow* window, int entered) {
    if (entered == 0)
        context->onMouseLeave();
}

void maximizeCallback(GLFWwindow* window, int maximized) {
    context->onWindowMaximize(maximized);
}

void init(GLFWwindow* window, const string& fileName) {
    glewInit();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    context = new Context(window, fileName);

    utils::initTimer();
    utils::setOnWindowResize((OnWindowResizeListener*) (context));
    utils::checkWindowSize(window);

    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetWindowMaximizeCallback(window, maximizeCallback);
}

int main([[maybe_unused]] int argc, char** args) {
    if (argc == 0) {
        throw std::exception();
    }

    int width = 640;
    int height = 480;

    GLFWwindow* window = createWindow(width, height, "Hello World", true);
    if (window == nullptr) return -1;

    init(window, args[1]);

    while (!glfwWindowShouldClose(window)) {
        utils::checkWindowSize(window);
        context->update();
        context->render();
        glfwSwapBuffers(window);
        utils::updateTimer();
        glfwPollEvents();
//        glfwWaitEvents();
    }

    glfwTerminate();
    delete context;
    return 0;
}