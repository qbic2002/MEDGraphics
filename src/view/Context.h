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
#include <string>
#include <vector>

namespace view {
    class RootView;

    class Context : public utils::OnWindowResizeListener {
    public:
        AbstractRaster* raster;
        GLuint textureId;

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

        GLFWwindow* getWindowId() const;

        bool isMaximized() const;

    private:
        RootView* rootView;
        GLFWwindow* window;
        bool maximized = false;
    };

} // view

#endif //MEDGRAPHICS_CONTEXT_H
