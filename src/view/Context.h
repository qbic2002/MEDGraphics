//
// Created by danil on 17.09.2022.
//

#ifndef MEDGRAPHICS_CONTEXT_H
#define MEDGRAPHICS_CONTEXT_H

#include "BgRenderer.h"
#include "ImageView.h"
#include "../utils/windowSize.h"
#include "../img/AbstractRaster.h"

namespace view {

    class Context : public utils::OnWindowResizeListener {
    public:
        explicit Context(const std::string& fileName);

        void update();

        void render();

        void onWindowResize(unsigned int width, unsigned int height) override;

        ~Context();

        AbstractRaster* raster;
        GLuint textureId;

    private:
        BgRenderer bgRenderer;
        ImageView imageView;
    };

} // view

#endif //MEDGRAPHICS_CONTEXT_H
