//
// Created by danil on 08.11.2022.
//

#include "view/Style.h"

namespace view {
    Dimension::Dimension() noexcept: parentK(0), pixelK(0) {}

    Dimension::Dimension(float pixelK) noexcept: parentK(0), pixelK(pixelK) {}

    Dimension::Dimension(float parentK, float pixelK) noexcept: parentK(parentK), pixelK(pixelK) {}

    float Dimension::evaluate(float parentValue) const {
        return parentK * parentValue + pixelK;
    }

    Dimension Dimension::operator+(const Dimension& other) const {
        return {parentK + other.parentK, pixelK + other.pixelK};
    }

    Dimension Dimension::operator-() const {
        return {-parentK, -pixelK};
    }

    Dimension Dimension::operator-(const Dimension& other) const {
        return {parentK - other.parentK, pixelK - other.pixelK};
    }

    Dimension Dimension::operator*(const float& scalar) const {
        return {parentK * scalar, pixelK * scalar};
    }

    padding::padding() : padding(0) {}

    padding::padding(float value) : padding(value, value) {}

    padding::padding(float horizontal, float vertical) : left(horizontal), top(vertical), right(horizontal),
                                                         bottom(vertical) {
    }

    padding::padding(float left, float top, float right, float bottom) : left(left), top(top), right(right),
                                                                         bottom(bottom) {}

    border::border() : border(0) {}

    border::border(float value) : border(value, value) {}

    border::border(float horizontal, float vertical) : left(horizontal), top(vertical), right(horizontal),
                                                       bottom(vertical) {
    }

    border::border(float left, float top, float right, float bottom) : left(left), top(top), right(right),
                                                                       bottom(bottom) {}

    border& border::setColor(rgba _color) {
        color = _color;
        return *this;
    }

    Background& Background::setImage(const std::filesystem::path& fileName) {
        image = assets::texture(fileName);
        return *this;
    }

    Background& Background::setColor(const rgba& _color) {
        color = _color;
        return *this;
    }

    Background& Background::setFontColor(const rgba& _color) {
        color = _color;
        return *this;
    }

    Background& Background::edit(const std::function<void(Background&)>& editor) {
        editor(*this);
        return *this;
    }

    Style::Style() : stateDefault(), stateHover(), statePress() {}

    Style& Style::set(const StyleState& style) {
        stateDefault = stateHover = statePress = style;
        return *this;
    }

    Style& Style::forEach(const std::function<void(StyleState&)>& editor) {
        editor(stateDefault);
        editor(stateHover);
        editor(statePress);
        return *this;
    }

    Style& Style::edit(const std::function<void(Style&)>& editor) {
        editor(*this);
        return *this;
    }

    CalculatedPos::CalculatedPos(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

    CalculatedPos::CalculatedPos(const CalculatedPos& parentPos, const position& pos) :
            x(parentPos.x + pos.x.evaluate(parentPos.width)),
            y(parentPos.y + pos.y.evaluate(parentPos.height)),
            width(pos.width.evaluate(parentPos.width)),
            height(pos.height.evaluate(parentPos.height)) {}
}
