//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_STYLE_H
#define MEDGRAPHICS_STYLE_H

#include <iostream>
#include "../img/rgba.h"
#include "../gl/Texture.h"
#include "Assets.h"
#include "../utils/R.h"

namespace view {
    struct Dimension {
        float parentK;
        float pixelK;

        Dimension() noexcept: parentK(0), pixelK(0) {}

        Dimension(float pixelK) noexcept: parentK(0), pixelK(pixelK) {}

        Dimension(float parentK, float pixelK) noexcept: parentK(parentK), pixelK(pixelK) {}

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

        Dimension operator*(const float& scalar) const {
            return {parentK * scalar, pixelK * scalar};
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

    struct border {
        float left, top, right, bottom;
        rgba color{};

        border() : border(0) {}

        explicit border(float value) : border(value, value) {}

        border(float horizontal, float vertical) : left(horizontal), top(vertical), right(horizontal),
                                                   bottom(vertical) {
        }

        border(float left, float top, float right, float bottom) : left(left), top(top), right(right),
                                                                   bottom(bottom) {}

        border& setColor(rgba _color) {
            color = _color;
            return *this;
        }
    };

    class Background {
    public:
        rgba color{};
        std::shared_ptr<gl::Texture> image;

        Background& setImage(const std::string& fileName) {
            image = assets::texture(fileName);
            return *this;
        }

        Background& setColor(const rgba& _color) {
            color = _color;
            return *this;
        }

        Background& setFontColor(const rgba& _color) {
            color = _color;
            return *this;
        }

        Background& edit(const std::function<void(Background&)>& editor) {
            editor(*this);
            return *this;
        }
    };

    struct StyleState {
        position position{};
        padding padding{};
        border border{};
        Background background{};
        rgba fontColor{COLOR_FONT_PRIMARY};
        std::shared_ptr<gl::FontRenderer> fontRenderer;
    };


    struct Style {
        StyleState stateDefault;
        StyleState stateHover;
        StyleState statePress;
        bool isDraggable{};

        Style() : stateDefault(), stateHover(), statePress() {}

        Style(const StyleState& style) : stateDefault(style), stateHover(style), statePress(style) {}

        Style& set(const StyleState& style) {
            stateDefault = stateHover = statePress = style;
            return *this;
        }

        Style& forEach(const std::function<void(StyleState&)>& editor) {
            editor(stateDefault);
            editor(stateHover);
            editor(statePress);
            return *this;
        }

        Style& edit(const std::function<void(Style&)>& editor) {
            editor(*this);
            return *this;
        }
    };

    struct CalculatedPos {
        float x, y, width, height;

        CalculatedPos() = default;

        CalculatedPos(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

        CalculatedPos(const CalculatedPos& parentPos, const position& pos) :
                x(parentPos.x + pos.x.evaluate(parentPos.width)),
                y(parentPos.y + pos.y.evaluate(parentPos.height)),
                width(pos.width.evaluate(parentPos.width)),
                height(pos.height.evaluate(parentPos.height)) {}
    };
}

#endif //MEDGRAPHICS_STYLE_H
