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

namespace view {
    class RootView;

    struct FileImageData {
        GLuint textureId;
        unsigned width;
        unsigned height;
        std::string fileName;
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

        const FileImageData& getCurrentImageData();

        const std::vector<FileImageData>& getImageList();

        void addImageChangedListener(const std::function<void()>& listener);

    private:
        void loadImagesFromDirectory();

        std::string directoryName;
        std::vector<FileImageData> imageList;
        int imageIndex = 0;
        RootView* rootView;
        GLFWwindow* window;
        bool maximized = false;
        std::vector<std::function<void()>> onImageChangedListeners;
    };

} // view

#endif //MEDGRAPHICS_CONTEXT_H
