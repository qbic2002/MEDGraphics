//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_H
#define MEDGRAPHICS_CONTEXT_H

#include "../utils/windowSize.h"
#include "../img/AbstractRaster.h"
#include "../gl/ShaderProgram.h"
#include <string>
#include <vector>

namespace view {
    class View;

    class BgRenderer;

    class Context : public utils::OnWindowResizeListener {
    public:
        Context(GLFWwindow* window, const std::string& fileName);

        Context(const Context& other) = delete;

        Context& operator=(const Context& other) = delete;

        void update();

        void render();

        void onWindowResize(unsigned int width, unsigned int height) override;

        void onMouseMove(double x, double y);

        ~Context();

        AbstractRaster* raster;
        GLuint textureId;

        void onMouseButton(int button, int action, int mods);

    private:
        std::vector<View*> views;
        BgRenderer* bgRenderer;
        GLFWwindow* window;
    };

} // view

#endif //MEDGRAPHICS_CONTEXT_H
