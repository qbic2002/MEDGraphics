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
        ImageView(Context* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        void render() override;

        ~ImageView();

    private:
        ShaderProgram* shader = new ShaderProgram("assets/shaders/default.vert", "assets/shaders/default.frag");
    };

} // view

#endif //MEDGRAPHICS_IMAGEVIEW_H
