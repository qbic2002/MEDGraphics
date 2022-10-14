//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_H
#define MEDGRAPHICS_CONTEXT_H

#include "../utils/windowSize.h"
#include "../img/AbstractRaster.h"
#include "../gl/ShaderProgram.h"
#include "../gl/Texture.h"
#include "ClickEvent.h"
#include "../gl/FontRenderer.h"
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <thread>
#include <ctime>
#include "../img/imageLoader.h"

namespace view {

#define PREVIEW_IMG_COUNT 5
    static_assert(PREVIEW_IMG_COUNT % 2 == 1, "PREVIEW_IMG_COUNT must be odd");
#define IMG_COUNT 5
    static_assert(IMG_COUNT % 2 == 1, "IMG_COUNT must be odd");

    class ViewGroup;

    struct FileImageData {
        AbstractRaster* raster = nullptr;
        std::mutex* mutex = nullptr;

        AbstractRaster* compressedRaster = nullptr;
        GLuint compressedTextureId = 0;
        unsigned width = 0;
        unsigned height = 0;
        std::string fileName;
        std::filesystem::path path;
        bool isBroken = false;
    };

    class Context : public utils::OnWindowResizeListener {
    public:
        Context(GLFWwindow* window, const std::string& fileName, const std::string& root);

        Context(const Context& other) = delete;

        Context(const Context&& other) = delete;

        Context& operator=(const Context& other) = delete;

        Context& operator=(const Context&& other) = delete;

        ~Context();

        void update();

        void render() const;

        void onWindowResize(unsigned int width, unsigned int height) override;

        void onMouseMove(double x, double y);

        void onWindowMaximize(int _maximized);

        void onMouseButton(ClickEvent& event);

        void onMouseLeave();

        void onScroll(double xOffset, double yOffset, double cursorX, double cursorY);

        void onKey(int key, int scancode, int action, int mods);

        GLFWwindow* getWindowId() const;

        bool isMaximized() const;

        static unsigned getWindowWidth();

        static unsigned getWindowHeight();

        void nextImage();

        void previousImage();

        void chooseImage(int index);

        const FileImageData& getCurrentImageData() const;

        std::vector<FileImageData>& getImageList();

        void addImageChangedListener(const std::function<void()>& listener);

        GLuint getCurrentTextureId() const;

        int getImageIndex() const;

        ViewGroup* getRootView();

        void saveImage();

        void openImage();

        void openImage(const std::string& fileName);

        GLuint getCorruptedId() const;

        const std::string& getRootDirectory() const;

        void showError(const std::string& message);

    private:
        GLuint corruptedId;
        int corruptedHeight = 0;
        int corruptedWidth = 0;

        void loadPreviewsFromDirectoryMethod();

        void fillImageListFileNames();

        std::mutex bgMutex;

        void loadNearImageData();

        void loadNearDataThreadMethod();

        std::string directoryName;
        std::vector<FileImageData> imageList;
        int imageIndex = 0;
        GLuint currentTextureId = 0;
        ViewGroup* rootView;
        GLFWwindow* window;
        bool maximized = false;
        std::vector<std::function<void()>> onImageChangedListeners;

        std::time_t timeOfLoading;

        std::atomic_bool isWorking = true;
        std::atomic_bool isImageSwitched = true;

        std::string rootDirectory;
    };

} // view

#endif //MEDGRAPHICS_CONTEXT_H
