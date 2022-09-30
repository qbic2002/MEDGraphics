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

namespace view {

#define PREVIEW_IMG_COUNT 5
    static_assert(PREVIEW_IMG_COUNT % 2 == 1, "PREVIEW_IMG_COUNT must be odd");

    class RootView;

    struct FileImageData {
        GLuint compressedTextureId = 0;
        unsigned width;
        unsigned height;
        std::string fileName;
        std::filesystem::path path;
    };

    class Context : public utils::OnWindowResizeListener {
    public:
        Context(GLFWwindow* window, const std::string& fileName);

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

        const std::vector<FileImageData>& getImageList() const;

        void addImageChangedListener(const std::function<void()>& listener);

        GLuint getCurrentTextureId() const;

        int getImageIndex() const;

    private:
        void loadPreviewsFromDirectory();

        void fillImageListFileNames();

        std::string directoryName;
        std::vector<FileImageData> imageList;
        int imageIndex = 0;
        GLuint currentTextureId = 0;
        RootView* rootView;
        GLFWwindow* window;
        bool maximized = false;
        std::vector<std::function<void()>> onImageChangedListeners;
    };

} // view

#endif //MEDGRAPHICS_CONTEXT_H
