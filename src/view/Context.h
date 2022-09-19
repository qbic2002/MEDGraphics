//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_H
#define MEDGRAPHICS_CONTEXT_H

#include "../utils/windowSize.h"
#include "../img/AbstractRaster.h"
#include <string>
#include <vector>

namespace view {
    class View;

    class Context : public utils::OnWindowResizeListener {
    public:
        Context(const std::string& fileName);

        Context(const Context& other) = delete;

        Context& operator=(const Context& other) = delete;

        void update();

        void render();

        void onWindowResize(unsigned int width, unsigned int height) override;

        ~Context();

        AbstractRaster* raster;
        GLuint textureId;

    private:
        std::vector<View*> views;
    };

} // view

#endif //MEDGRAPHICS_CONTEXT_H
