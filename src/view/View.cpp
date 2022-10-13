//
// Created by danil on 18.09.2022.
//

#include "View.h"
#include "ClickEvent.h"

namespace view {
    void glColor(const rgba& rgba) {
        glColor4ubv((GLubyte*) &rgba);
    }

    void glPositionQuad(const CalculatedPos& pos) {
        glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x + pos.width, pos.y);
        glVertex2f(pos.x + pos.width, pos.y + pos.height);
        glVertex2f(pos.x, pos.y + pos.height);
        glEnd();
    }

    void glTextureQuad(const CalculatedPos& pos, const padding& padding) {
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(pos.x + padding.left, pos.y + padding.bottom);
        glTexCoord2f(1, 0);
        glVertex2f(pos.x + pos.width - padding.right, pos.y + padding.bottom);
        glTexCoord2f(1, 1);
        glVertex2f(pos.x + pos.width - padding.right, pos.y + pos.height - padding.top);
        glTexCoord2f(0, 1);
        glVertex2f(pos.x + padding.left, pos.y + pos.height - padding.top);
        glEnd();
    }

    void View::renderBackground() {
        rgba curBgColor = styleState->background.color;
        if (curBgColor.a != 0) {
            glDisable(GL_TEXTURE_2D);
            glColor(curBgColor);
            glPositionQuad(calculatedPos);
            glEnable(GL_TEXTURE_2D);
            glColor4f(1, 1, 1, 1);
        }
        gl::Texture* bgImage = styleState->background.image.get();
        if (bgImage != nullptr) {
            bgImage->bind();
            glTextureQuad(calculatedPos, styleState->padding);
            bgImage->unbind();
        }
    }

    void View::render() {
        renderBackground();
    }

    Style& View::getStyle() {
        return style;
    }

    bool View::onClick(const ClickEvent& event) {
        if (event.button == 0 && onClickListener != nullptr) {
            if (event.action == GLFW_RELEASE) {
                onClickListener();
            }
            return true;
        }
        return false;
    }

    View& View::setOnClickListener(const std::function<void()>& _onClickListener) {
        onClickListener = _onClickListener;
        return *this;
    }

    View& View::setOnClickListener(const std::function<void()>&& _onClickListener) {
        onClickListener = _onClickListener;
        return *this;
    }

    void View::onWindowResize(unsigned int width, unsigned int height) {
        if (onWindowResizeListener)
            onWindowResizeListener(*this, width, height);
    }

    View& View::setOnWindowResizeListener(std::function<void(View& v, unsigned int w, unsigned int h)>&& listener) {
        onWindowResizeListener = listener;
        return *this;
    }

    bool View::isInside(double x, double y) {
        return calculatedPos.x <= x && x < calculatedPos.x + calculatedPos.width
               && calculatedPos.y <= y && y < calculatedPos.y + calculatedPos.height;
    }

    void View::onMouseEnter() {
        setState(HOVERED);
    }

    void View::onMouseLeave() {
        setState(DEFAULT);
    }

    void View::onMouseMove(double x, double y) {}

    bool View::onScroll(double xOffset, double yOffset, double d, double d1) {
        return false;
    }

    void View::onMeasure(const CalculatedPos& parentPos) {
        calculatedPos = CalculatedPos(parentPos, styleState->position);
    }

    bool View::onDrag(double x, double y, double dx, double dy) {
        return false;
    }

    const Context* View::getContext() const {
        return context;
    }

    int View::getId() const {
        return id;
    }

    View* View::setId(int _id) {
        this->id = _id;
        return this;
    }

    View* View::findViewById(int _id) {
        if (this->id == _id)
            return this;

        return nullptr;
    }

    const StyleState* View::getStyleState() const {
        return styleState;
    }

    State View::getState() const {
        return state;
    }

    void View::setState(State _state) {
        state = _state;
        switch (state) {
            case DEFAULT:
                styleState = &style.stateDefault;
                break;
            case HOVERED:
                styleState = &style.stateHover;
                break;
            case PRESSED:
                styleState = &style.statePress;
                break;
        }
    }
} // view