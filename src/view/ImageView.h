//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGEVIEW_H
#define MEDGRAPHICS_IMAGEVIEW_H

#include "../gl/ShaderProgram.h"
#include "View.h"

namespace view {

    class ImageView : public View {
    public:
        ImageView(Context* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        explicit ImageView(Context* _context);

        void render() override;

        ~ImageView() override;

    private:
        ShaderProgram* shader = new ShaderProgram("default");
    };

} // view

#endif //MEDGRAPHICS_IMAGEVIEW_H
