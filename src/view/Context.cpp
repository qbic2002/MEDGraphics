//
// Created by danil on 17.09.2022.
//

#include "Context.h"
#include "../widget/RootView.h"
#include "../utils/logging.h"
#include "../utils/explorerUtils.h"
#include "../utils/file.h"
#include "../pnm/utils/pnmUtil.h"
#include "../widget/MessageView.h"

namespace view {

    Context::Context(GLFWwindow* window, const fs::path& appDir) : window(window), appDir(appDir) {
        rootView = new RootView(this, Style{{.position = {0, 0, FILL_PARENT, FILL_PARENT}}});
        info("Context loaded");
    }

    void Context::update() {
        imageFileStorage.update();
    }

    void Context::render() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);

        rootView->render();

        glDisable(GL_TEXTURE_2D);
    }

    void Context::onWindowResize(unsigned int width, unsigned int height) {
        glLoadIdentity();
        glOrtho(0, width, height, 0.01, -100, 100);
        rootView->onMeasure({0, 0, (int) width, (int) height});
        rootView->onWindowResize(width, height);
    }

    void Context::onWindowMaximize(int _maximized) {
        maximized = _maximized;
    }

    Context::~Context() {
        delete rootView;
    }

    bool grabbing = false;
    double prevX, prevY;
    double grabX, grabY;
    int grabWindowX, grabWindowY;

    void Context::onMouseMove(double x, double y) {
        double dx = x - prevX;
        double dy = y - prevY;
        prevX = x;
        prevY = y;
        rootView->onMouseMove(x, y);
        if (grabbing) {
            if (rootView->onDrag(x, y, dx, dy)) {
                grabX = prevX;
                grabY = prevY;
                return;
            }
            if (!maximized) {
                glfwGetWindowPos(window, &grabWindowX, &grabWindowY);
                glfwSetWindowPos(window, grabWindowX + x - grabX, grabWindowY + y - grabY);
            }
        }
    }

    void Context::onMouseButton(ClickEvent& event) {
        if (event.action == GLFW_RELEASE && grabbing)
            grabbing = false;

        if (rootView->onClick(event))
            return;

        if (event.action != GLFW_PRESS)
            return;

        grabbing = true;
        grabX = prevX;
        grabY = prevY;
        glfwGetWindowPos(window, &grabWindowX, &grabWindowY);
    }

    void Context::onMouseLeave() {
        rootView->onMouseLeave();
        grabbing = false;
    }

    void Context::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {
        rootView->onScroll(xOffset, yOffset, cursorX, cursorY);
    }

    void Context::onKey(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
        if (action == GLFW_RELEASE || action == GLFW_REPEAT) {
            switch (key) {
                case GLFW_KEY_LEFT:
                    imageFileStorage.prev();
                    break;
                case GLFW_KEY_RIGHT:
                    imageFileStorage.next();
                    break;
                default:
                    break;
            }
        }
    }

    GLFWwindow* Context::getWindowId() const {
        return window;
    }

    bool Context::isMaximized() const {
        return maximized;
    }

    void Context::saveImage() {
        if (imageFileStorage.getCurImageFile() == nullptr) {
            showError(L"File is not opened");
            return;
        }
        if (imageFileStorage.getCurImageFile()->raster == nullptr) {
            showError(L"Image has not been load yet");
            return;
        }
        std::wstring filename;
        if (!utils::getSaveFileName(filename))
            return;
        try {
            auto os = utils::openFileOStream(filename);
            pnm::writePNMImage(PNMImage(imageFileStorage.getCurImageFile()->raster), os);
        } catch (std::exception&) {
            showError(L"Error with saving =(");
        }
    }

    ViewGroup* Context::getRootView() {
        return rootView;
    }

    void Context::openImage() {
        std::wstring fileName;
        if (!utils::getOpenFileName(fileName))
            return;
        imageFileStorage.open(fileName);
    }

    void Context::showError(const String& message) {
        auto* msgView = (view::MessageView*) rootView->findViewById(MESSAGE_VIEW_ID);
        msgView->showMessage(message);
    }

    ImageFileStorage& Context::getImageFileStorage() {
        return imageFileStorage;
    }

    const fs::path& Context::getAppDir() const {
        return appDir;
    }
} // view