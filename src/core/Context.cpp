//
// Created by danil on 17.09.2022.
//

#include "Context.h"
#include "../widget/RootView.h"
#include "../utils/explorerUtils.h"
#include "../utils/file.h"

void Context::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    rootView->render();

    glDisable(GL_TEXTURE_2D);
}

void Context::onWindowResize(unsigned int width, unsigned int height) {
    info() << "context on Window Resize: " << width << " " << height;
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(0, width, height, 0.01, -100, 100);
    rootView->onMeasure({0, 0, (int) width, (int) height});
    rootView->onWindowResize(width, height);
}

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
        windowWrapper->pollEvents();
        update();
        render();
        windowWrapper->swapBuffers();
        utils::updateTimer();
//        glfwWaitEvents();
    }
}

Context::~Context() {
    delete rootView;
}

view::ViewGroup* Context::getRootView() {
    return rootView;
}

const fs::path& Context::getAppDir() const {
    return appDir;
}

void Context::setRootView(view::ViewGroup* _rootView) {
    delete rootView;
    rootView = _rootView;
    int width, height;
    windowWrapper->getWindowSize(&width, &height);
    rootView->onMeasure({0, 0, width, height});
    rootView->onWindowResize(width, height);
}

WindowWrapper* Context::getWindowWrapper() const {
    return windowWrapper;
}
