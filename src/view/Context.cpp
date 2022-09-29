//
// Created by danil on 17.09.2022.
//

#include "../img/imageLoader.h"
#include "../gl/utils.h"
#include "Context.h"
#include "../widget/RootView.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace view {

    Context::Context(GLFWwindow* _window, const std::string& fileName) {
        window = _window;

        if (fs::is_directory(fileName)) {
            directoryName = fileName;
            loadImagesFromDirectory();
        } else {
            fs::path path = fs::canonical(fs::exists(fileName) ? fileName : "assets/qbic.ppm");
            directoryName = path.parent_path().string();
            loadImagesFromDirectory();

            for (int i = 0; i < imageList.size(); i++) {
                const auto& imageFileData = imageList[i];
                if (imageFileData.fileName == path.string()) {
                    imageIndex = i;
                    break;
                }
            }
        }

        rootView = new RootView(this, Style{.position = {0, 0, FILL_PARENT, FILL_PARENT}});
        chooseImage(imageIndex);
    }

    void Context::update() {

    }

    void Context::render() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, imageList[imageIndex].textureId);
        rootView->render();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    void Context::onWindowResize(unsigned int width, unsigned int height) {
        glLoadIdentity();
        glOrtho(0, width, height, 0, -100, 100);
        rootView->onMeasure({0, 0, (int) width, (int) height});
        rootView->onWindowResize(width, height);
    }

    void Context::onWindowMaximize(int _maximized) {
        maximized = _maximized;
    }

    Context::~Context() {
        delete rootView;
        for (auto& image: imageList) {
            glDeleteTextures(1, &image.textureId);
        }
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

        if (rootView->onClick(event)) {
            return;
        }

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

    void Context::onKey(int key, int scancode, int action, int mods) {
        if (action == GLFW_RELEASE || action == GLFW_REPEAT) {
            switch (key) {
                case GLFW_KEY_LEFT:
                    previousImage();
                    break;
                case GLFW_KEY_RIGHT:
                    nextImage();
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

    unsigned Context::getWindowWidth() {
        return utils::getWindowWidth();
    }

    unsigned Context::getWindowHeight() {
        return utils::getWindowHeight();
    }

    void Context::loadImagesFromDirectory() {
        for (const auto& file: fs::directory_iterator(directoryName)) {
            if (fs::is_directory(file))
                continue;

            auto* raster = img::loadImageData(file.path().string());
            auto textureId = gl::loadTexture(raster, GL_CLAMP, GL_LINEAR, GL_NEAREST);
            imageList.push_back(
                    {textureId, (unsigned) raster->getWidth(), (unsigned) raster->getHeight(),
                     canonical(file.path()).string()});
            delete raster;
        }
    }

    void Context::nextImage() {
        chooseImage(imageIndex + 1);
    }

    void Context::previousImage() {
        chooseImage(imageIndex - 1);
    }

    const FileImageData& Context::getCurrentImageData() {
        return imageList[imageIndex];
    }

    void Context::chooseImage(int index) {
        if (index < 0)
            index = imageList.size() - 1;
        if (index >= imageList.size())
            index = 0;
        imageIndex = index;
        for (const auto& listener: onImageChangedListeners) {
            listener();
        }
    }
} // view