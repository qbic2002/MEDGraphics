//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_STYLE_H
#define MEDGRAPHICS_STYLE_H

#include <iostream>
#include <img/rgba.h>
#include <utils/Texture.h>
#include <view/core/Assets.h>
#include "../../../src/core/R.h"

namespace view {
    struct Dimension {
        float pixel = 0;
        float contentK = 0;
        float parentK = 0;
        float parentSpareK = 0;

        Dimension() noexcept;

        Dimension(float pixel) noexcept;

        Dimension(float pixel, float contentK, float parentK, float parentSpareK) noexcept;

        float evaluate(float parentValue) const;

        Dimension operator+(const Dimension& other) const;

        Dimension operator-() const;

        Dimension operator-(const Dimension& other) const;

        Dimension operator*(const float& scalar) const;

        Dimension operator/(const float& scalar) const;
    };

    const Dimension FILL_PARENT{0, 0, 1, 0};
    const Dimension WRAP_CONTENT{0, 1, 0, 0};
    const Dimension FILL_SPARE{0, 0, 0, 1};

    struct position {
        Dimension x, y, width, height;
    };

    struct padding {
        float left, top, right, bottom;

        padding();

        explicit padding(float value);

        padding(float horizontal, float vertical);

        padding(float left, float top, float right, float bottom);

        float width() {
            return left + right;
        }

        float height() {
            return top + bottom;
        }
    };

    struct border {
        float left, top, right, bottom;
        rgba color{};

        border();

        explicit border(float value);

        border(float horizontal, float vertical);

        border(float left, float top, float right, float bottom);

        border& setColor(rgba _color);

        float width() {
            return left + right;
        }

        float height() {
            return top + bottom;
        }
    };

    class Background {
    public:
        rgba color{};
        std::shared_ptr<gl::Texture> image;

        Background& setImage(const std::filesystem::path& fileName);

        Background& setColor(const rgba& _color);

        Background& setFontColor(const rgba& _color);

        Background& edit(const std::function<void(Background&)>& editor);
    };

    struct StyleState {
        position position{};
        padding padding{};
        border border{};
        Background background{};
        rgba fontColor{COLOR_FONT_PRIMARY};
        std::shared_ptr<gl::FontRenderer> fontRenderer;
        Dimension width, height;
    };

    struct Style {
        StyleState stateDefault;
        StyleState stateHover;
        StyleState statePress;
        bool isDraggable{};

        Style();

        explicit Style(const StyleState& style) : stateDefault(style), stateHover(style), statePress(style) {}

        Style& set(const StyleState& style);

        Style& forEach(const std::function<void(StyleState&)>& editor);

        Style& edit(const std::function<void(Style&)>& editor);
    };

    struct CalculatedPos {
        float x, y, width, height;

        CalculatedPos() = default;

        CalculatedPos(int x, int y, int width, int height);

        CalculatedPos(const CalculatedPos& parentPos, const position& pos);
    };
}

#endif //MEDGRAPHICS_STYLE_H
