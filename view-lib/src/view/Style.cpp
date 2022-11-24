//
// Created by danil on 08.11.2022.
//

#include "view/Style.h"
#include "view/View.h"
#include "utils/gl_utils.h"

namespace view {
    Dimension::Dimension() noexcept: pixel(0), contentK(0), parentK(0), parentSpareK(0) {}

    Dimension::Dimension(float pixel_) noexcept: pixel(pixel_), contentK(0), parentK(0), parentSpareK(0) {}

    Dimension::Dimension(float pixel_, float contentK, float parentK, float parentSpareK) noexcept
            : pixel(pixel_), contentK(contentK), parentK(parentK), parentSpareK(parentSpareK) {}

    float Dimension::evaluate(float parentValue) const {
        return parentK * parentValue + pixel;
    }

    float Dimension::evaluate(float parentValue, float parentSpare) const {
        return pixel + parentK * parentValue + parentSpareK * parentSpare;
    }

    float Dimension::evaluateContent(float contentSize) const {
        return pixel + contentK * contentSize;
    }

    bool Dimension::operator==(const Dimension& other) const {
        return pixel == other.pixel
               && contentK == other.contentK
               && parentK == other.parentK
               && parentSpareK == other.parentSpareK;
    }

    bool Dimension::operator!=(const Dimension& other) const {
        return !this->operator==(other);
    }

    Dimension Dimension::operator+(const Dimension& other) const {
        return {pixel + other.pixel,
                contentK + other.contentK,
                parentK + other.parentK,
                parentSpareK + other.parentSpareK};
    }

    Dimension Dimension::operator-() const {
        return {-pixel, -contentK, -parentK, -parentSpareK};
    }

    Dimension Dimension::operator-(const Dimension& other) const {
        return {pixel - other.pixel,
                contentK - other.contentK,
                parentK - other.parentK,
                parentSpareK - other.parentSpareK};
    }

    Dimension Dimension::operator*(const float& scalar) const {
        return {pixel * scalar, contentK * scalar, parentK * scalar, parentSpareK * scalar};
    }

    Dimension Dimension::operator/(const float& scalar) const {
        return *this * (1 / scalar);
    }

    Padding::Padding() : Padding(0) {}

    Padding::Padding(float value) : Padding(value, value) {}

    Padding::Padding(float horizontal, float vertical) : left(horizontal), top(vertical), right(horizontal),
                                                         bottom(vertical) {}

    Padding::Padding(float left, float top, float right, float bottom) : left(left), top(top), right(right),
                                                                         bottom(bottom) {}

    BackgroundWrapper::BackgroundWrapper() : background(new Background()) {}

    void Background::draw(const View* view, float left, float top, float right, float bottom) {}

    void ColorBackground::draw(const View* view, float left, float top, float right, float bottom) {
        if (color.a != 0) {
            glDisable(GL_TEXTURE_2D);
            glColor(color);
            glPositionQuad(left, top, right, bottom);
            glEnable(GL_TEXTURE_2D);
            glColor4f(1, 1, 1, 1);
        }
    }

    StateBackground::StateBackground(const BackgroundWrapper& bgDefault, const BackgroundWrapper& bgHovered,
                                     const BackgroundWrapper& bgPressed)
            : bgDefault(bgDefault), bgHovered(bgHovered), bgPressed(bgPressed) {}

    void StateBackground::draw(const View* view, float left, float top, float right, float bottom) {
        switch (view->getState()) {
            case DEFAULT:
                bgDefault.get().draw(view, left, top, right, bottom);
                break;
            case HOVERED:
                bgHovered.get().draw(view, left, top, right, bottom);
                break;
            case PRESSED:
                bgPressed.get().draw(view, left, top, right, bottom);
                break;
        }
    }
}
