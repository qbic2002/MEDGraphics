//
// Created by danil on 23.09.2022.
//

#include "ViewGroup.h"

namespace view {
    ViewGroup::ViewGroup(Context* context, const Style& style) : View(context, style) {}

    ViewGroup::~ViewGroup() {
        for (auto* child: children) {
            delete child;
        }
    }

    bool ViewGroup::onClick(const ClickEvent& event) {
        for (View* child: children) {
            if (child->isInside(event.x, event.y)) {
                child->state == event.action == GLFW_PRESS ? PRESSED : HOVERED;
                if (child->onClick(event)) {
                    return true;
                }
            }
        }
        return View::onClick(event);
    }

    void ViewGroup::onWindowResize(unsigned int width, unsigned int height) {
        for (View* child: children) {
            child->onWindowResize(width, height);
        }
        View::onWindowResize(width, height);
    }

    void ViewGroup::onMouseLeave() {
        for (View* child: children) {
            if (child->state != DEFAULT) {
                child->onMouseLeave();
            }
        }
        View::onMouseLeave();
    }

    void ViewGroup::onMouseMove(double x, double y) {
        for (View* child: children) {
            if (child->isInside(x, y)) {
                if (child->state == DEFAULT) {
                    child->onMouseEnter();
                }
                child->onMouseMove(x, y);
            } else {
                if (child->state != DEFAULT) {
                    child->onMouseLeave();
                }
            }
        }
        View::onMouseMove(x, y);
    }

    void ViewGroup::addChild(View* view) {
        children.push_back(view);
    }

    void ViewGroup::render() {
        View::render();
        for (auto* child: children) {
            child->render();
        }
    }
} // view