//
// Created by danil on 10.10.2022.
//

#ifndef MEDGRAPHICS_DROPDOWNVIEW_H
#define MEDGRAPHICS_DROPDOWNVIEW_H

#include "../view/ViewGroup.h"
#include "TextView.h"

namespace view {

    class DropDownViewElementPrototype {
    public:
        explicit DropDownViewElementPrototype(const std::string& text,
                                              const std::function<void()>& onClickListener = nullptr)
                : text(text), onClickListener(onClickListener) {}

        std::string text;
        std::function<void()> onClickListener;
    };

    class DropDownViewElement : public TextView {
    public:
        DropDownViewElement(
                Context* context,
                const Style& style,
                const std::string& text) : TextView(context, style, text) {}
    };

    class DropDownView : public ViewGroup {
    public:
        DropDownView(
                Context* context,
                const Style& style,
                const std::vector<DropDownViewElementPrototype>& prototypes,
                const Style& elementsStyle
        ) : ViewGroup(context, style) {
            for (const auto& prototype: prototypes) {
                auto* view = new DropDownViewElement(context, elementsStyle, prototype.text);
                view->setOnClickListener(prototype.onClickListener);
                addChild(view);
            }
        }

        void onMeasure(const view::CalculatedPos& parentPos) override {
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

        bool isInside(double x, double y) override {
            if (!isOpened)
                return false;

            return View::isInside(x, y);
        }

        void render() override {
            if (!isOpened)
                return;

            ViewGroup::render();
        }

        void toggleOpened() {
            isOpened = !isOpened;
        }

    protected:
        bool isOpened = false;
    };

} // view

#endif //MEDGRAPHICS_DROPDOWNVIEW_H
