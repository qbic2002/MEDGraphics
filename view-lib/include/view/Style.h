//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_STYLE_H
#define MEDGRAPHICS_STYLE_H

#include <iostream>
#include <img/rgba.h>
#include <utils/Texture.h>
#include <view/core/Assets.h>

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

        float evaluate(float parentValue, float parentSpare) const;

        float evaluateContent(float contentSize) const;

        bool operator==(const Dimension& other) const;

        bool operator!=(const Dimension& other) const;

        Dimension operator+(const Dimension& other) const;

        Dimension operator-() const;

        Dimension operator-(const Dimension& other) const;

        Dimension operator*(const float& scalar) const;

        Dimension operator/(const float& scalar) const;
    };

    const Dimension FILL_PARENT{0, 0, 1, 0};
    const Dimension WRAP_CONTENT{0, 1, 0, 0};
    const Dimension FILL_SPARE{0, 0, 0, 1};

    struct Padding {
        float left, top, right, bottom;

        Padding();

        explicit Padding(float value);

        Padding(float horizontal, float vertical);

        Padding(float left, float top, float right, float bottom);

        float width() const {
            return left + right;
        }

        float height() const {
            return top + bottom;
        }
    };

    class View;

    class Background {
    public:
        virtual void draw(const View* view, float left, float top, float right, float bottom);
    };

    class BackgroundWrapper {
    public:
        template<typename BackgroundType>
        BackgroundWrapper(const BackgroundType& bg);

        BackgroundWrapper();

        Background& get() {
            return *background;
        }

    private:
        std::shared_ptr<Background> background;
    };

    template<typename BackgroundType>
    BackgroundWrapper::BackgroundWrapper(const BackgroundType& bg) : background(new BackgroundType(bg)) {}

    class ColorBackground : public Background {
    public:
        ColorBackground(rgba color) : Background(), color(color) {}

        void draw(const View* view, float left, float top, float right, float bottom) override;

    protected:
        rgba color;
    };

    class StateBackground : public Background {
    public:
        StateBackground(const BackgroundWrapper& bgDefault, const BackgroundWrapper& bgHovered,
                        const BackgroundWrapper& bgPressed);

        void draw(const View* view, float left, float top, float right, float bottom) override;

    protected:
        BackgroundWrapper bgDefault;
        BackgroundWrapper bgHovered;
        BackgroundWrapper bgPressed;
    };

    struct Style {
        bool isDraggable = false;
    };
}

#endif //MEDGRAPHICS_STYLE_H
