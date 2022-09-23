//
// Created by danil on 18.09.2022.
//

#include "View.h"
#include "ClickEvent.h"

namespace view {
    void glColor(const rgba& rgba) {
        glColor4ubv((GLubyte*) &rgba);
    }

    void glPositionQuad(const position& pos) {
        glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x + pos.width, pos.y);
        glVertex2f(pos.x + pos.width, pos.y + pos.height);
        glVertex2f(pos.x, pos.y + pos.height);
        glEnd();
    }

    void glTextureQuad(const position& pos, const padding& padding) {
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
        rgba curBgColor;
        switch (state) {
            case DEFAULT:
                curBgColor = style.background.color;
                break;
            case HOVERED:
                curBgColor = style.background.colorOnHover;
                break;
            case PRESSED:
                curBgColor = style.background.colorOnPress;
                break;
        }
        if (curBgColor.a != 0) {
            glDisable(GL_TEXTURE_2D);
            glColor(curBgColor);
            glPositionQuad(style.position);
            glEnable(GL_TEXTURE_2D);
            glColor4f(1, 1, 1, 1);
        }
        gl::Texture* bgImage = style.background.image.get();
        if (bgImage != nullptr) {
            bgImage->bind();
            glTextureQuad(style.position, style.padding);
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

    View& View::setOnClickListener(std::function<void()>&& _onClickListener) {
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
        position& pos = style.position;
        return pos.x <= x && x <= pos.x + pos.width
               && pos.y <= y && y <= pos.y + pos.height;
    }

    void View::onMouseEnter() {
        state = HOVERED;
    }

    void View::onMouseLeave() {
        state = DEFAULT;
    }

    void View::onMouseMove(double x, double y) {}
} // view