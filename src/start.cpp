#define STB_IMAGE_IMPLEMENTATION

#include "utils/R.h"
#include "stb_image.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "utils/measureFps.h"
#include "utils/windowSize.h"
#include "pnm/PNMImage.h"
#include "gl/ShaderProgram.h"
#include "core/Context.h"
#include "utils/encoding.h"
#include "utils/logging.h"
#include "utils/OsLock.h"

using namespace std;
using namespace utils;
using namespace view;
namespace fs = std::filesystem;

Context* context;

GLFWwindow* createWindow(int width, int height, const char* title, bool vsync) {
    GLFWwindow* window;

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_DECORATED, 0);
    glfwWindowHint(GLFW_VISIBLE, 0);
    auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }
    glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);
    glfwShowWindow(window);
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

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    context->onScroll(xOffset, yOffset, cursorPos.x, cursorPos.y);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    context->onKey(key, scancode, action, mods);
}

void init(GLFWwindow* window, const wstring& fileName, const fs::path& appDir) {
    glewInit();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    context = new Context(window, appDir);
    context->getImageFileStorage().open(fileName);

    utils::initTimer();
    utils::setOnWindowResize((OnWindowResizeListener*) (context));
    utils::checkWindowSize(window);

    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetWindowMaximizeCallback(window, maximizeCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
}

int main([[maybe_unused]] int argc, char** argv) {
    HANDLE_SIGSEGV
    utils::configureUtf8();
    auto args = utils::readArgs(argc, argv);
    int width = 640;
    int height = 480;

    GLFWwindow* window = createWindow(width, height, "Hello World", true);
    if (window == nullptr)
        exceptional_exit("Could not create window");

    init(window, args[1], std::filesystem::path(args[0]).parent_path());

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