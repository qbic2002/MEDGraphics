//
// Created by danil on 17.09.2022.
//

#include "../img/imageLoader.h"
#include "../gl/utils.h"
#include "Context.h"
#include "../widget/RootView.h"
#include "../img/Raster.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace view {

    Context::Context(GLFWwindow* _window, const std::string& fileName) {
        window = _window;

        if (fs::is_directory(fileName)) {
            directoryName = fileName;
            fillImageListFileNames();
            for (auto& item: bgTextureIds) item = 0;
            loadPreviewsFromDirectory();
        } else {
            fs::path path = fs::canonical(fs::exists(fileName) ? fileName : "assets/qbic.ppm");
            directoryName = path.parent_path().string();

            fillImageListFileNames();

            for (int i = 0; i < imageList.size(); i++) {
                const auto& imageFileData = imageList[i];
                if (imageFileData.fileName == path.string()) {
                    imageIndex = i;
                    break;
                }
            }

            for (auto& item: bgTextureIds) item = 0;
            loadPreviewsFromDirectory();
        }

        rootView = new RootView(this,
                                Style{.position = {0, 0, FILL_PARENT, FILL_PARENT}});
        chooseImage(imageIndex);
    }

    void Context::update() {

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
        for (auto& image: imageList) {
//            delete image.raster;
            glDeleteTextures(1, &image.compressedTextureId);
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

    void Context::loadPreviewsFromDirectory() {
        GLuint newIds[PREVIEW_IMG_COUNT];
        for (auto& item: newIds) item = 0;
        for (int i = imageIndex - PREVIEW_IMG_COUNT / 2; i <= imageIndex + PREVIEW_IMG_COUNT / 2; ++i) {
            int index = ((i % imageList.size()) + imageList.size()) % imageList.size();

            if (imageList[index].compressedTextureId != 0) {
                newIds[i - (imageIndex - PREVIEW_IMG_COUNT / 2)] = imageList[index].compressedTextureId;
                continue;
            }
            auto* raster = img::loadImageData(imageList[index].path.string());
            if (raster == nullptr) {
                imageList.erase(imageList.begin() + index);
                --i;
                continue;
            }


            auto compressedRaster = raster->compress(
                    (raster->getWidth() <= 40) ? raster->getWidth() : 40,
                    (raster->getHeight() <= 40) ? raster->getHeight() : 40);

            auto compressedTextureId = gl::loadTexture(&compressedRaster, GL_CLAMP, GL_LINEAR, GL_NEAREST);
            imageList[index] =
                    {compressedTextureId, (unsigned) raster->getWidth(), (unsigned) raster->getHeight(),
                     canonical(imageList[index].path).string()};
            delete raster;
            newIds[i - (imageIndex - PREVIEW_IMG_COUNT / 2)] = imageList[index].compressedTextureId;
        }

        addBgTextureIds(newIds);
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

        if (currentTextureId != 0)
            glDeleteTextures(1, &currentTextureId);

        try {
            auto* raster = img::loadImageData(imageList[imageIndex].fileName);
            if (raster == nullptr) {
                imageList.erase(imageList.begin() + index);
                chooseImage(index);
                return;
            }
            currentTextureId = gl::loadTexture(raster, GL_CLAMP, GL_LINEAR, GL_NEAREST);
            delete raster;
        } catch (const std::exception& e) {
            currentTextureId = 0;
        }

        loadPreviewsFromDirectory();

        for (const auto& listener: onImageChangedListeners) {
            listener();
        }
    }

    void Context::addImageChangedListener(const std::function<void()>& listener) {
        onImageChangedListeners.push_back(listener);
    }

    const std::vector<FileImageData>& Context::getImageList() {
        return imageList;
    }

    GLuint Context::getCurrentTextureId() const {
        return currentTextureId;
    }

    void Context::fillImageListFileNames() {
        for (const auto& file: fs::directory_iterator(directoryName)) {
            if (fs::is_directory(file))
                continue;

            imageList.push_back(
                    {0, 0, 0, canonical(file.path()).string(), file.path()}
            );
        }
    }

    const GLuint* Context::getBgTextureIds() const {
        return bgTextureIds;
    }

    void Context::addBgTextureIds(GLuint* newIds) {
        if (std::all_of(bgTextureIds, bgTextureIds + PREVIEW_IMG_COUNT, [](GLuint a) { return a == 0; })) {
            std::copy(newIds, newIds + PREVIEW_IMG_COUNT, bgTextureIds);
            return;
        }

        bool f = false;
        for (const auto& item: bgTextureIds) {
            if (item == newIds[0]) {
                f = true;
                break;
            }
        }

        if (!f) {
            bgTextureIds[PREVIEW_IMG_COUNT - 1] = newIds[0];
            return;
        }

        f = false;
        for (const auto& item: bgTextureIds) {
            if (item == newIds[PREVIEW_IMG_COUNT - 1]) {
                f = true;
                break;
            }
        }

        if (!f) {
            bgTextureIds[0] = newIds[PREVIEW_IMG_COUNT - 1];
            return;
        }
    }
} // view