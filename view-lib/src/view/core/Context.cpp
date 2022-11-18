//
// Created by danil on 17.09.2022.
//

#include <view/core/Context.h>
#include <view/ViewGroup.h>
#include <utils/explorer_utils.h>
#include <utils/file.h>

Context::~Context() {
    delete rootView;
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

void Context::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    rootView->render();

    glDisable(GL_TEXTURE_2D);
}

void Context::onCreated(const std::vector<std::wstring>& args) {}

void Context::update() {}

view::View* Context::getRootView() {
    return rootView;
}

void Context::setRootView(view::View* _rootView) {
    delete rootView;
    rootView = _rootView;
    int width, height;
    windowWrapper->getWindowSize(&width, &height);
    remeasureRootView(width, height);
    rootView->onWindowResize(width, height);
}

const fs::path& Context::getAppDir() const {
    return appDir;
}

WindowWrapper* Context::getWindowWrapper() const {
    return windowWrapper;
}

bool Context::onDrag(double x, double y, double dx, double dy) {
    return rootView->onDrag(x, y, dx, dy);
}

bool Context::onMouseButton(ClickEvent& event) {
    return rootView->onClick(event);
}

void Context::onMouseLeave() {
    rootView->onMouseLeave();
}

void Context::onMouseMove(double x, double y) {
    rootView->onMouseMove(x, y);
}

void Context::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {
    rootView->onScroll(xOffset, yOffset, cursorX, cursorY);
}

void Context::onWindowResize(unsigned int width, unsigned int height) {
    info() << "context on Window Resize: " << width << " " << height;
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(0, width, height, 0.01, -100, 100);
    remeasureRootView(width, height);
    rootView->onWindowResize(width, height);
}

void Context::remeasureRootView(int width, int height) {
    //    rootView->onMeasure({0, 0, (int) width, (int) height});
    auto req = rootView->howMuchSpaceRequired();
    rootView->useThisSpace(0, 0, req.width + req.parentPartW * width, req.height + req.parentPartH * height);
}
