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

//        const std::string& getText() const {
//            return text;
//        }

//    private:
//        std::string text;
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
            for (const auto* child: children) {
                auto* element = (DropDownViewElement*) child;
                auto* childStyle = &element->getStyle();
                float width = childStyle->fontRenderer->getTextWidth(element->getText());
                if (maxStringWidth < width) {
                    maxStringWidth = width;
                }
                float childHeight = childStyle->padding.top + childStyle->padding.bottom +
                                    childStyle->fontRenderer->getLineHeight();
                childStyle->position.height = childHeight;
                childStyle->position.y = childrenHeight;
                childrenHeight += childHeight;
            }

            float childHorPadding = children[0]->getStyle().padding.left + children[0]->getStyle().padding.right;

            style.position.width = maxStringWidth + childHorPadding;
            style.position.height = childrenHeight;

            ViewGroup::onMeasure(parentPos);
//            CalculatedPos(
//            const CalculatedPos& parentPos,
//            const position& pos) :
//            x(pos.x.evaluate(parentPos.width)),
//                    y(pos.y.evaluate(parentPos.height)),
//                    width(pos.width.evaluate(parentPos.width)),
//                    height(pos.height.evaluate(parentPos.height))
//            {}

//            calculatedPos = {
//                    style.position.x.evaluate(parentPos)
//            };

//            for (const auto* child: children) {
//                auto* element = (DropDownViewElement*) child;
//                element->onMeasure();
////                float width = element->getStyle().fontRenderer->getTextWidth(element->getText());
////                if (maxWidth < width) {
////                    maxWidth = width;
////                }
//            }
        }
    };

} // view

#endif //MEDGRAPHICS_DROPDOWNVIEW_H
