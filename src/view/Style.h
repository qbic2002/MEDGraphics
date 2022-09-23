//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_STYLE_H
#define MEDGRAPHICS_STYLE_H

#include <iostream>
#include "../img/rgba.h"
#include "../gl/Texture.h"
#include "Assets.h"

namespace view {
    struct position {
        unsigned int x, y, width, height;
    };

    struct padding {
        float left;
        float top;
        float right;
        float bottom;

        padding() : padding(0) {}

        explicit padding(float value) : padding(value, value) {}

        padding(float horizontal, float vertical) : left(horizontal), top(vertical), right(horizontal),
                                                    bottom(vertical) {
        }

        padding(float left, float top, float right, float bottom) : left(left), top(top), right(right),
                                                                    bottom(bottom) {}
    };

    class Background {
    public:
        rgba color{};
        rgba colorOnHover{};
        rgba colorOnPress{};
        std::shared_ptr<gl::Texture> image;

        Background& setImage(const std::string& fileName) {
            image = assets::texture(fileName);
            return *this;
        }

        Background& setColor(const rgba& _color) {
            color = _color;
            return *this;
        }

        Background& setColorOnHover(const rgba& _colorOnHover) {
            colorOnHover = _colorOnHover;
            return *this;
        }

        Background& setColorOnPress(const rgba& _colorOnPress) {
            colorOnPress = _colorOnPress;
            return *this;
        }
    };

    struct Style {
        position position{};
        padding padding{};
        Background background{};
    };
}

#endif //MEDGRAPHICS_STYLE_H
