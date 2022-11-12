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

    WindowWrapper* getWindowWrapper() const;

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

    /// @return root view of the context
    view::ViewGroup* getRootView();

    /// @return current context's appDir - path to executable file directory
    const fs::path& getAppDir() const;

    void onWindowResize(unsigned int width, unsigned int height) override;

    void setRootView(view::ViewGroup* rootView);

protected:
    fs::path appDir;
    view::ViewGroup* rootView = nullptr;
    GLFWWindowProvider windowProvider;
    WindowWrapper* windowWrapper = nullptr;
};

#endif //MEDGRAPHICS_CONTEXT_H
