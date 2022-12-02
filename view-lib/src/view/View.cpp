//
// Created by danil on 18.09.2022.
//

#include "view/View.h"

namespace view {

    View::View(Context* context, const ViewAttributes& attr) : context(context) {
        VIEW_ATTRS_SET(attr)
    }

    void View::draw() {
        context->clipArea(edges.left, edges.top, edges.right, edges.bottom);
        background.get().draw(this, edges.left, edges.top, edges.right, edges.bottom);
        onDraw();
        needRerender = false;
    }

    void View::onDraw() {}

    bool View::onMouseEvent(const MouseEvent& event) {
        if (onMouseEventListener != nullptr) {
            onMouseEventListener(this, event);
            return true;
        }
        if (event.action == GLFW_PRESS) {
            setState(PRESSED);
        } else if (event.action == GLFW_RELEASE && state == PRESSED) {
            state = HOVERED;
            if (onClick(event))
                return true;
        }
        return false;
    }

    bool View::onClick(const MouseEvent& event) {
        if (onClickListener != nullptr) {
            onClickListener(this);
            return true;
        }
        return false;
    }

    void View::setOnMouseEventListener(const OnMouseEventListener& listener) {
        onMouseEventListener = listener;
    }

    View& View::setOnClickListener(const OnClickListener& _onClickListener) {
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

    bool View::onKey(int key, int scancode, int action, int mods) {
        return false;
    }

    bool View::onChar(unsigned int codepoint) {
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
        if (state == _state)
            return;
        state = _state;
        invalidate();
    }

    Style& View::getStyle() {
        return style;
    }

    SpaceRequirement View::measure() {
        if (isMeasured)
            return measuredSpaceRequirement;
        measuredSpaceRequirement = onMeasure();
        isMeasured = true;
        return measuredSpaceRequirement;
    }

    float View::getContentWidth() {
        return 0;
    }

    float View::getContentHeight() {
        return 0;
    }

    SpaceRequirement View::onMeasure() {
        return {width.evaluateContent(getContentWidth()) + padding.width(),
                height.evaluateContent(getContentHeight()) + padding.height(),
                width.parentK,
                height.parentK,
                width.parentSpareK,
                height.parentSpareK};
    }

    void View::layout(float left, float top, float right, float bottom) {
        edges.left = left;
        edges.top = top;
        edges.right = right;
        edges.bottom = bottom;
        innerEdges.left = left + padding.left;
        innerEdges.top = top + padding.top;
        innerEdges.right = right - padding.right;
        innerEdges.bottom = bottom - padding.bottom;
        onLayout(left, top, right, bottom);
    }

    void View::setParent(View* parent) {
        if (this->parent != nullptr) {
            throw std::runtime_error("View already has a parent");
        }
        this->parent = parent;
    }

    bool View::isDirty() const {
        return needRerender;
    }

    void View::invalidate() {
        isMeasured = false;
        needRerender = true;
        if (parent != nullptr) {
            parent->invalidate();
        }
    }

    void View::setBackground(const BackgroundWrapper& background) {
        this->background = background;
        invalidate();
    }

    View* View::getParent() {
        return parent;
    }

    void View::setVisibility(Visibility _visibility) {
        if (visibility == _visibility)
            return;
        visibility = _visibility;
        invalidate();
    }

    Visibility View::getVisibility() {
        return visibility;
    }
} // view