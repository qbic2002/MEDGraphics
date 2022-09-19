//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGEVIEW_H
#define MEDGRAPHICS_IMAGEVIEW_H

#include "ShaderProgram.h"
#include "View.h"

namespace view {

    class ImageView : public View {
    public:
        void render() override;

        ~ImageView();

    private:
        ShaderProgram* shader = new ShaderProgram("assets/shaders/default.vert", "assets/shaders/default.frag");
    };

} // view

#endif //MEDGRAPHICS_IMAGEVIEW_H
