//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_H
#define MEDGRAPHICS_CONTEXT_H

#include <filesystem>
#include "ContextCallbacks.h"
#include "GLFWWindowProvider.h"
#include "utils/encoding.h"
#include "utils/measureFps.h"
#include "../../../../src/core/R.h"

namespace fs = std::filesystem;
typedef unsigned int uint;
namespace view {
    class View;
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

    virtual void onCreated(const std::vector<std::wstring>& args);

    /// Called before each onDraw
    virtual void update();

    /// Render current window state
    virtual void draw() const;

    WindowWrapper* getWindowWrapper() const;

    /// @return current context's appDir - path to executable file directory
    const fs::path& getAppDir() const;

    /// @return root view of the context
    view::View* getRootView();

    void setRootView(view::View* _rootView);

    bool onDrag(double x, double y, double dx, double dy) override;

    bool onMouseButton(ClickEvent& event) override;

    void onMouseLeave() override;

    void onMouseEnter() override;

    void onMouseMove(double x, double y) override;

    void onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

    void onWindowResize(unsigned int width, unsigned int height) override;

protected:
    void remeasureRootView(int width, int height);

    fs::path appDir;
    view::View* rootView = nullptr;
    GLFWWindowProvider windowProvider;
    WindowWrapper* windowWrapper = nullptr;
};

#endif //MEDGRAPHICS_CONTEXT_H
