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
#include "RootViewManager.h"

namespace fs = std::filesystem;
typedef unsigned int uint;

class Context : public RootViewManager {
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

    void setRootView(view::View* _rootView);

    void onWindowResize(unsigned int width, unsigned int height) override;

    void clipArea(float left, float top, float right, float bottom);

protected:
    fs::path appDir;
    GLFWWindowProvider windowProvider;
    WindowWrapper* windowWrapper = nullptr;
};

#endif //MEDGRAPHICS_CONTEXT_H
