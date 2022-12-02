//
// Created by danil on 23.09.2022.
//

#include "view/ViewGroup.h"

namespace view {
    ViewGroup::ViewGroup(Context* context, const ViewGroupAttributes& attr)
            : View(context, (const ViewAttributes&) attr) {
        VIEW_GROUP_ATTRS_SET(attr)
    }

    ViewGroup::~ViewGroup() {
        for (auto* child: children) {
            delete child;
        }
        View::~View();
    }

    bool ViewGroup::onMouseEvent(const MouseEvent& event) {
        for (View* child: children) {
            if (child->getVisibility() != VISIBLE)
                continue;
            if (child->isInside(event.x, event.y)) {
                if (child->onMouseEvent(event)) {
                    return true;
                }
            }
        }
        return View::onMouseEvent(event);
    }

    bool ViewGroup::onClick(const MouseEvent& event) {
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
            if (child->getVisibility() != VISIBLE)
                continue;
            if (child->getState() != DEFAULT) {
                child->onMouseLeave();
            }
        }
        View::onMouseLeave();
    }

    void ViewGroup::onMouseMove(double x, double y) {
        for (View* child: children) {
            if (child->getVisibility() != VISIBLE)
                continue;
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
        if (view == nullptr) {
            throw std::runtime_error("view is null");
        }
        view->setParent(this);
        children.push_back(view);
    }

    void ViewGroup::onDraw() {
        for (auto* child: children) {
            if (child->getVisibility() != VISIBLE)
                continue;
            child->draw();
        }
    }

    bool ViewGroup::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {
        for (auto* child: children) {
            if (child->getVisibility() != VISIBLE)
                continue;
            if (child->isInside(cursorX, cursorY)) {
                if (child->onScroll(xOffset, yOffset, cursorX, cursorY)) {
                    return true;
                }
            }
        }

        return View::onScroll(xOffset, yOffset, cursorX, cursorY);
    }

    bool ViewGroup::onDrag(double x, double y, double dx, double dy) {
        for (auto* child: children) {
            if (child->getVisibility() != VISIBLE)
                continue;
            if (child->isInside(x, y) && child->getState() == PRESSED) {
                if (child->onDrag(x, y, dx, dy))
                    return true;
            }
        }
        return View::onDrag(x, y, dx, dy);
    }

    bool ViewGroup::onKey(int key, int scancode, int action, int mods) {
        for (auto* child: children) {
            if (child->getVisibility() == VISIBLE && child->onKey(key, scancode, action, mods)) {
                return true;
            }
        }
        return false;
    }

    bool ViewGroup::onChar(unsigned int codepoint) {
        for (auto* child: children) {
            if (child->getVisibility() == VISIBLE && child->onChar(codepoint)) {
                return true;
            }
        }
        return false;
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