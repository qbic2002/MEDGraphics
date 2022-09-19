//
// Created by danil on 18.09.2022.
//

#include "View.h"

namespace view {
    unsigned int View::getX() const {
        return x;
    }

    unsigned int View::getY() const {
        return y;
    }

    unsigned int View::getWidth() const {
        return width;
    }

    unsigned int View::getHeight() const {
        return height;
    }

    View::View(Context* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
            : context(context), x(x), y(y), width(width), height(height) {}
} // view