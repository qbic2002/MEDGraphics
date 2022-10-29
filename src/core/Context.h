//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_H
#define MEDGRAPHICS_CONTEXT_H

#include "../utils/windowSize.h"
#include "../utils/R.h"
#include "../view/ClickEvent.h"
#include "ImageFileStorage.h"

namespace fs = std::filesystem;
namespace view {
    class ViewGroup;
}

class Context : public utils::OnWindowResizeListener {
public:
    Context(GLFWwindow* window, const fs::path& appDir);

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

    view::ViewGroup* getRootView();

    void saveImage();

    void openImage();

    void showError(const String& message);

    ImageFileStorage& getImageFileStorage();

    const fs::path& getAppDir() const;

private:
    GLFWwindow* window;
    fs::path appDir;
    bool maximized = false;
    view::ViewGroup* rootView;
    ImageFileStorage imageFileStorage;
};

#endif //MEDGRAPHICS_CONTEXT_H
