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
        }
        loadPreviewsFromDirectory();

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
            delete image.raster;
            delete image.compressedRaster;
            if (image.compressedTextureId != 0) {
                glDeleteTextures(1, &image.compressedTextureId);
            }
            pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(image.mutex));
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
        std::thread loadPreviewsThread(&Context::loadPreviewsFromDirectoryMethod, std::ref(*this));
        loadPreviewsThread.detach();
    }

    void Context::loadPreviewsFromDirectoryMethod() {
        bgMutex.lock();
        for (int i = imageIndex - PREVIEW_IMG_COUNT / 2; i <= imageIndex + PREVIEW_IMG_COUNT / 2; ++i) {
            int index = ((i % imageList.size()) + imageList.size()) % imageList.size();

            if (imageList[index].compressedRaster != nullptr) {
                continue;
            }
            auto* raster = img::loadImageData(imageList[index].path.string());
            if (raster == nullptr) {
                raster = img::loadImageData("assets/qbic.ppm");
            }
            auto* compressedRaster = new Raster<RGBAPixel>(raster->compress(
                    (raster->getWidth() <= 40) ? raster->getWidth() : 40,
                    (raster->getHeight() <= 40) ? raster->getHeight() : 40));

//            auto compressedTextureId = gl::loadTexture(compressedRaster, GL_CLAMP, GL_LINEAR, GL_NEAREST);
            imageList[index].compressedRaster = compressedRaster;
            delete raster;
        }
        std::cout << "ura!\n";
        bgMutex.unlock();
    }

    void Context::nextImage() {
        chooseImage(imageIndex + 1);
    }

    void Context::previousImage() {
        chooseImage(imageIndex - 1);
    }

    const FileImageData& Context::getCurrentImageData() const {
        return imageList[imageIndex];
    }

    void Context::chooseImage(int index) {
        while (clock() - timeOfLoading < 100) {}

        if (index < 0)
            index = imageList.size() - 1;
        if (index >= imageList.size())
            index = 0;
        imageIndex = index;

        if (currentTextureId != 0) {
            glDeleteTextures(1, &currentTextureId);
        }

        loadNearImageData();
        loadPreviewsFromDirectory();

        try {
            while (imageList[imageIndex].raster == nullptr) {
            }
            imageList[imageIndex].mutex->lock();
            currentTextureId = gl::loadTexture(imageList[imageIndex].raster, GL_CLAMP, GL_LINEAR, GL_NEAREST);
            imageList[imageIndex].mutex->unlock();
            timeOfLoading = clock();

        } catch (const std::exception& e) {
            currentTextureId = 0;
        }

        for (const auto& listener: onImageChangedListeners) {
            listener();
        }

    }

    void Context::addImageChangedListener(const std::function<void()>& listener) {
        onImageChangedListeners.push_back(listener);
    }

    std::vector<FileImageData>& Context::getImageList() {
        return imageList;
    }

    GLuint Context::getCurrentTextureId() const {
        return currentTextureId;
    }

    void Context::fillImageListFileNames() {
        for (const auto& file: fs::directory_iterator(directoryName)) {
            if (fs::is_directory(file))
                continue;

            if (img::isImage(canonical(file.path()).string()))
                imageList.push_back(
                        {nullptr, new std::mutex, nullptr, 0, 0, 0, canonical(file.path()).string(), file.path()}
                );
        }
    }

    int Context::getImageIndex() const {
        return imageIndex;
    }

    void Context::loadNearImageData() {
        std::thread loadNearThread(&Context::loadNearDataThreadMethod, std::ref(*this));
        loadNearThread.detach();
    }

    void Context::loadNearDataThreadMethod() {

        imageList[imageIndex].mutex->lock();
        if (imageList[imageIndex].raster == nullptr) {
            imageList[imageIndex].raster = img::loadImageData(imageList[imageIndex].fileName);
            if (imageList[imageIndex].raster == nullptr) {
                imageList[imageIndex].isBroken = true;
                imageList[imageIndex].raster = img::loadImageData("assets/qbic.ppm");
            }
            imageList[imageIndex].width = imageList[imageIndex].raster->getWidth();
            imageList[imageIndex].height = imageList[imageIndex].raster->getHeight();
        }
        std::cout << "loaded " << imageList[imageIndex].fileName << "\n";
        imageList[imageIndex].mutex->unlock();

        int rightIndex = (((imageIndex + IMG_COUNT / 2 + 1) % imageList.size()) + imageList.size()) % imageList.size();
        if (imageList.size() > IMG_COUNT && imageList[rightIndex].raster != nullptr) {
            delete imageList[rightIndex].raster;
            imageList[rightIndex].raster = nullptr;
            std::cout << "unloaded " << imageList[rightIndex].fileName << "\n";
        }

        int leftIndex = (((imageIndex - IMG_COUNT / 2 - 1) % imageList.size()) + imageList.size()) % imageList.size();
        if (imageList.size() > IMG_COUNT && imageList[leftIndex].raster != nullptr) {
            delete imageList[leftIndex].raster;
            imageList[leftIndex].raster = nullptr;
            std::cout << "unloaded " << imageList[leftIndex].fileName << "\n";
        }

        for (int i = 1; i <= IMG_COUNT / 2; ++i) {
            int index = (((i + imageIndex) % imageList.size()) + imageList.size()) % imageList.size();
            imageList[index].mutex->lock();
            if (imageList[index].raster != nullptr) {
                imageList[index].mutex->unlock();
                continue;
            }
            std::cout << "try load " << imageList[index].fileName << "\n";
            imageList[index].raster = img::loadImageData(imageList[index].fileName);
            if (imageList[index].raster == nullptr) {
                imageList[index].isBroken = true;
                imageList[index].raster = img::loadImageData("assets/qbic.ppm");
            }
            imageList[index].width = imageList[index].raster->getWidth();
            imageList[index].height = imageList[index].raster->getHeight();
            std::cout << "loaded " << imageList[index].fileName << " " << imageList[index].isBroken << "\n";
            imageList[index].mutex->unlock();
        }

        for (int i = 1; i <= IMG_COUNT / 2; ++i) {
            int index = (((imageIndex - i) % imageList.size()) + imageList.size()) % imageList.size();
            imageList[index].mutex->lock();
            if (imageList[index].raster != nullptr) {
                imageList[index].mutex->unlock();
                continue;
            }

            std::cout << "try load " << imageList[index].fileName << "\n";
            imageList[index].raster = img::loadImageData(imageList[index].fileName);
            if (imageList[index].raster == nullptr) {
                imageList[index].isBroken = true;
                imageList[index].raster = img::loadImageData("assets/qbic.ppm");
            }
            imageList[index].width = imageList[index].raster->getWidth();
            imageList[index].height = imageList[index].raster->getHeight();
            std::cout << "loaded " << imageList[index].fileName << " " << imageList[index].isBroken << "\n";
            imageList[index].mutex->unlock();
        }

        std::cout << "povezlo, potok offnulsya sam!\n";
    }
} // view