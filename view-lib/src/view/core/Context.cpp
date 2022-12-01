//
// Created by danil on 17.09.2022.
//

#include <view/core/Context.h>
#include <view/ViewGroup.h>
#include <utils/explorer_utils.h>
#include <utils/file.h>

Context::~Context() = default;

void Context::run(int argc, char** argv, uint width, uint height, const std::string& title, bool vsync) {
    utils::configureUtf8();
    auto args = utils::readArgs(argc, argv);
    appDir = std::filesystem::path(args[0]).parent_path();

    windowWrapper = windowProvider.createWindow(this, width, height, title, vsync);
    utils::initTimer();

    onCreated(args);

    loop();

    glfwTerminate();
}

void Context::loop() {
    while (!windowWrapper->shouldClose()) {
        windowWrapper->waitEvents(0.1);
        update();
        if (isDrawRequired()) {
            updateDirtyViews();
            draw();
            windowWrapper->swapBuffers();
            utils::updateTimer();
        }
    }
}

void Context::draw() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_SCISSOR_TEST);

    drawViews();

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_TEXTURE_2D);
}

void Context::onCreated(const std::vector<std::wstring>& args) {}

void Context::update() {}

void Context::setRootView(view::View* rootView) {
    int width, height;
    windowWrapper->getWindowSize(&width, &height);
    RootViewManager::setRootView(rootView, width, height);
}

const fs::path& Context::getAppDir() const {
    return appDir;
}

WindowWrapper* Context::getWindowWrapper() const {
    return windowWrapper;
}

void Context::onWindowResize(unsigned int width, unsigned int height) {
    info() << "context on Window Resize: " << width << " " << height;
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(0, width, height, 0.01, -100, 100);
    RootViewManager::onWindowResize(width, height);
}

void Context::clipArea(float left, float top, float right, float bottom) {
    int width, height;
    windowWrapper->getWindowSize(&width, &height);
    glScissor(left, height - bottom, right - left, bottom - top);
}
