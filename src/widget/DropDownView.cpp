//
// Created by danil on 10.10.2022.
//

#include "DropDownView.h"

namespace view {
    DropDownViewElementPrototype::DropDownViewElementPrototype(const String& text,
                                                               const std::function<void()>& onClickListener)
            : text(text), onClickListener(onClickListener) {}

    DropDownViewElement::DropDownViewElement(Context* context, const Style& style, const String& text) : TextView(
            context, style, text) {}

    DropDownView::DropDownView(Context* context,
                               const Style& style,
                               const std::vector<DropDownViewElementPrototype>& prototypes,
                               const Style& elementsStyle)
            : ViewGroup(context, style) {
        for (const auto& prototype: prototypes) {
            auto* view = new DropDownViewElement(context, elementsStyle, prototype.text);
            view->setOnClickListener(prototype.onClickListener);
            addChild(view);
        }
    }

    void DropDownView::onMeasure(const CalculatedPos& parentPos) {
        float maxStringWidth = 0;
        float childrenHeight = 0;
        for (auto* child: children) {
            auto* element = (DropDownViewElement*) child;
            auto* childStyle = element->getStyleState();
            float width = childStyle->fontRenderer->getTextWidth(element->getText());
            if (maxStringWidth < width) {
                maxStringWidth = width;
            }
            float childHeight = childStyle->padding.top + childStyle->padding.bottom +
                                childStyle->fontRenderer->getLineHeight();
            child->getStyle().forEach([&childHeight, &childrenHeight](StyleState& state) {
                state.position.height = childHeight;
                state.position.y = childrenHeight;
            });
            childrenHeight += childHeight;
        }

        float childHorPadding =
                children[0]->getStyleState()->padding.left + children[0]->getStyleState()->padding.right;

        style.forEach([&maxStringWidth, &childHorPadding, &childrenHeight](StyleState& state) {
            state.position.width = maxStringWidth + childHorPadding;
            state.position.height = childrenHeight;
        });

        ViewGroup::onMeasure(parentPos);
    }

    bool DropDownView::isInside(double x, double y) {
        if (!isOpened)
            return false;

        return View::isInside(x, y);
    }

    void DropDownView::onDraw() {
        if (!isOpened)
            return;

        ViewGroup::onDraw();
    }

    void DropDownView::toggleOpened() {
        isOpened = !isOpened;
    }

    bool DropDownView::onClick(const ClickEvent& event) {
        if (ViewGroup::onClick(event)) {
            if (event.action == GLFW_RELEASE)
                isOpened = false;
            return true;
        }
        return false;
    }
} // view