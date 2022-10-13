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
                child->setState(event.action == GLFW_PRESS ? PRESSED : HOVERED);
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
            if (child->getState() != DEFAULT) {
                child->onMouseLeave();
            }
        }
        View::onMouseLeave();
    }

    void ViewGroup::onMouseMove(double x, double y) {
        for (View* child: children) {
            if (child->isInside(x, y)) {
                if (child->getState() == DEFAULT) {
                    child->onMouseEnter();
                }
                child->onMouseMove(x, y);
            } else {
                if (child->getState() != DEFAULT) {
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

    bool ViewGroup::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {
        for (auto* child: children) {
            if (child->isInside(cursorX, cursorY)) {
                if (child->onScroll(xOffset, yOffset, cursorX, cursorY)) {
                    return true;
                }
            }
        }

        return View::onScroll(xOffset, yOffset, cursorX, cursorY);
    }

    void ViewGroup::onMeasure(const CalculatedPos& parentPos) {
        View::onMeasure(parentPos);
        for (auto* child: children) {
            child->onMeasure(calculatedPos);
        }
    }

    bool ViewGroup::onDrag(double x, double y, double dx, double dy) {
        for (auto* child: children) {
            if (child->isInside(x, y) && child->getState() == PRESSED) {
                if (child->onDrag(x, y, dx, dy))
                    return true;
            }
        }
        return View::onDrag(x, y, dx, dy);
    }

    View* ViewGroup::findViewById(int id) {
        for (auto* child: children) {
            View* found = child->findViewById(id);
            if (found)
                return found;
        }
        return View::findViewById(id);
    }
} // view