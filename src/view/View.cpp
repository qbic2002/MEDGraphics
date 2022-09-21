//
// Created by danil on 18.09.2022.
//

#include "View.h"

namespace view {
    void glColor(const rgba& rgba) {
        glColor4ubv((GLubyte*) &rgba);
    }

    unsigned int View::getX() const {
        return x;
    }

    unsigned int View::getY() const {
        return y;
    }

    unsigned int View::getWidth() const {
        return width;
    }

    unsigned int View::getHeight() const {
        return height;
    }

    View::View(Context* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
            : context(context), x(x), y(y), width(width), height(height) {}

    void View::render() {
        rgba curBgColor;
        switch (state) {
            case DEFAULT:
                curBgColor = background.color;
                break;
            case HOVERED:
                curBgColor = background.colorOnHover;
                break;
            case PRESSED:
                curBgColor = background.colorOnPress;
                break;
        }
        if (curBgColor.a != 0) {
            glDisable(GL_TEXTURE_2D);
            glColor(curBgColor);
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + width, y);
            glVertex2f(x + width, y + height);
            glVertex2f(x, y + height);
            glEnd();
            glEnable(GL_TEXTURE_2D);
            glColor4f(1, 1, 1, 1);
        }
        if (background.getImage() != nullptr) {
            background.getImage()->bind();
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex2f(x + padding.left, y + padding.bottom);
            glTexCoord2f(1, 0);
            glVertex2f(x + width - padding.right, y + padding.bottom);
            glTexCoord2f(1, 1);
            glVertex2f(x + width - padding.right, y + height - padding.top);
            glTexCoord2f(0, 1);
            glVertex2f(x + padding.left, y + height - padding.top);
            glEnd();
            background.getImage()->unbind();
        }
    }
} // view