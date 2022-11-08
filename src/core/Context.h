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
    /// @param window GLFW window handle
    /// @param appDir path to executable file where assets can be found
    Context(GLFWwindow* window, const fs::path& appDir);

    Context(const Context& other) = delete;

    Context(const Context&& other) = delete;

    Context& operator=(const Context& other) = delete;

    Context& operator=(const Context&& other) = delete;

    ~Context();

    /// Called before each render
    void update();

    /// Render current window state
    void render() const;

    /// Window resize event interface
    /// @param width new window width
    /// @param height new window height
    void onWindowResize(unsigned int width, unsigned int height) override;

    /// Pointer move event interface
    /// @param x new pointer x - relative
    /// @param y new pointer y - relative
    void onMouseMove(double x, double y);

    /// Window maximize/minimize event interface
    /// @param _maximized 0 - window was minimized, 1 - window was maximized
    void onWindowMaximize(int _maximized);

    /// Mouse button event interface
    /// @param event event to process
    void onMouseButton(ClickEvent& event);

    /// Pointer window leave event interface
    void onMouseLeave();

    /// Mouse scroll event interface
    /// @param xOffset scroll x distance
    /// @param yOffset scroll y distance
    /// @param cursorX window relative pointer x
    /// @param cursorY window relative pointer y
    void onScroll(double xOffset, double yOffset, double cursorX, double cursorY);

    /// Keyboard input event interface
    /// @param window The window that received the event.
    /// @param key The key that was pressed or released.
    /// @param scancode The system-specific scancode of the key.
    /// @param action `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`
    void onKey(int key, int scancode, int action, int mods);

    /// Get current context GLFW window id
    GLFWwindow* getWindowId() const;

    /// @return whether window is maximized
    bool isMaximized() const;

    /// @return root view of the context
    view::ViewGroup* getRootView();

    /// Run save image operation of the opened image
    void saveImage();

    /// Run open image operation from filesystem
    void openImage();

    /// Display message box with the message
    /// @param message Error message to be shown
    void showError(const String& message);

    /// @return current context's ImageFileStorage
    ImageFileStorage& getImageFileStorage();

    /// @return current context's appDir - path to executable file directory
    const fs::path& getAppDir() const;

private:
    GLFWwindow* window;
    fs::path appDir;
    bool maximized = false;
    view::ViewGroup* rootView;
    ImageFileStorage imageFileStorage;
};

#endif //MEDGRAPHICS_CONTEXT_H
