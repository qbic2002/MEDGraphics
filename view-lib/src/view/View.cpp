//
// Created by danil on 18.09.2022.
//

#include "view/View.h"

namespace view {

    void View::draw() {
        context->clipArea(edges.left, edges.top, edges.right, edges.bottom);
        background.get().draw(this, edges.left, edges.top, edges.right, edges.bottom);
        onDraw();
        needRerender = false;
    }

    void View::onDraw() {}

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

    State View::getState() const {
        return state;
    }

    void View::setState(State _state) {
        state = _state;
        invalidate();
    }
} // view