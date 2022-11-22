//
// Created by danil on 18.09.2022.
//

#include "view/View.h"
#include "view/ClickEvent.h"

namespace view {
    void glColor(const rgba& rgba) {
        glColor4ubv((GLubyte*) &rgba);
    }

    void glPositionQuad(float x1, float y1, float x2, float y2) {
        glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
        glEnd();
    }

    void glTextureQuad(float x1, float y1, float x2, float y2) {
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(x1, y1);
        glTexCoord2f(1, 0);
        glVertex2f(x2, y1);
        glTexCoord2f(1, 1);
        glVertex2f(x2, y2);
        glTexCoord2f(0, 1);
        glVertex2f(x1, y2);
        glEnd();
    }

    View::View(Context* context) : context(context), style() {}

    View::View(Context* context, const Style& style) : context(context), style(style) {}

    View::View(Context* context, const Style&& style) : context(context), style(style) {}

    void View::drawBackground() {
        rgba curBgColor = styleState->background.color;
        if (curBgColor.a != 0) {
            glDisable(GL_TEXTURE_2D);
            glColor(curBgColor);
            glPositionQuad(edges.left, edges.top, edges.right, edges.bottom);
            glEnable(GL_TEXTURE_2D);
            glColor4f(1, 1, 1, 1);
        }
        gl::Texture* bgImage = styleState->background.image.get();
        if (bgImage != nullptr) {
            bgImage->bind();
            glTextureQuad(edges.left, edges.top, edges.right, edges.bottom);
            bgImage->unbind();
        }
    }

    void View::draw() {
        drawBackground();
        onDraw();
        needRerender = false;
    }

    void View::onDraw() {}

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

    View& View::setOnWindowResizeListener(std::function<void(View& v, unsigned int w, unsigned int h)>&& listener) {
        onWindowResizeListener = listener;
        return *this;
    }

    void View::onWindowResize(unsigned int width, unsigned int height) {
        if (onWindowResizeListener)
            onWindowResizeListener(*this, width, height);
    }

    bool View::isInside(double x, double y) {
        return edges.left <= x && x < edges.right && edges.top <= y && y < edges.bottom;
    }

    void View::onMouseEnter() {
        setState(HOVERED);
    }

    void View::onMouseLeave() {
        setState(DEFAULT);
    }

    void View::onMouseMove(double x, double y) {}

    bool View::onScroll(double xOffset, double yOffset, double x, double y) {
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
        invalidate();
    }
} // view