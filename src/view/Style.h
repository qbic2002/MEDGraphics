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
    struct Dimension {
        float parentK;
        float pixelK;

        Dimension() : parentK(0), pixelK(0) {}

        Dimension(float pixelK) : parentK(0), pixelK(pixelK) {}

        Dimension(float parentK, float pixelK) : parentK(parentK), pixelK(pixelK) {}

        float evaluate(float parentValue) const {
            return parentK * parentValue + pixelK;
        }

        Dimension operator+(const Dimension& other) const {
            return {parentK + other.parentK, pixelK + other.pixelK};
        }

        Dimension operator-() const {
            return {-parentK, -pixelK};
        }

        Dimension operator-(const Dimension& other) const {
            return {parentK - other.parentK, pixelK - other.pixelK};
        }
    };

    const Dimension FILL_PARENT{1, 0};

    struct position {
        Dimension x, y, width, height;
    };

    struct padding {
        float left, top, right, bottom;

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

    struct CalculatedPos {
        float x, y, width, height;

        CalculatedPos() = default;

        CalculatedPos(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

        CalculatedPos(const CalculatedPos& parentPos, const position& pos) :
                x(pos.x.evaluate(parentPos.width)),
                y(pos.y.evaluate(parentPos.height)),
                width(pos.width.evaluate(parentPos.width)),
                height(pos.height.evaluate(parentPos.height)) {}
    };
}

#endif //MEDGRAPHICS_STYLE_H
