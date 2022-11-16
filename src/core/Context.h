//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_H
#define MEDGRAPHICS_CONTEXT_H

#include <filesystem>
#include "ContextCallbacks.h"
#include "GLFWWindowProvider.h"
#include "../utils/encoding.h"
#include "../utils/measureFps.h"
#include "../utils/R.h"

namespace fs = std::filesystem;
typedef unsigned int uint;
namespace view {
    class ViewGroup;
}

class Context : public ContextCallbacks {
public:
    Context() = default;

    Context(const Context& other) = delete;

    Context(const Context&& other) = delete;

    Context& operator=(const Context& other) = delete;

    Context& operator=(const Context&& other) = delete;

    ~Context();

    void run(int argc, char** argv, uint width, uint height, const std::string& title, bool vsync);

    void loop();

    virtual void onCreated(const std::vector<std::wstring>& args) {};

    /// Called before each render
    virtual void update() {};

    /// Render current window state
    virtual void render() const;

    WindowWrapper* getWindowWrapper() const;

    /// @return current context's appDir - path to executable file directory
    const fs::path& getAppDir() const;

    /// @return root view of the context
    view::ViewGroup* getRootView();

    void setRootView(view::ViewGroup* rootView);

    bool onDrag(double x, double y, double dx, double dy) override;

    bool onMouseButton(ClickEvent& event) override;

    void onMouseLeave() override;

    void onMouseMove(double x, double y) override;

    void onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

    void onWindowResize(unsigned int width, unsigned int height) override;

protected:
    fs::path appDir;
    view::ViewGroup* rootView = nullptr;
    GLFWWindowProvider windowProvider;
    WindowWrapper* windowWrapper = nullptr;
};

#endif //MEDGRAPHICS_CONTEXT_H
