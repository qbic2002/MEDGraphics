//
// Created by danil on 23.11.2022.
//

#include "DropDownView.h"

namespace view {
    DropDownViewOption::DropDownViewOption(const String& text, const std::function<void()>& onClickListener)
            : text(text), onClickListener(onClickListener) {}

    DropDownView::DropDownView(Context* context, const ViewGroupAttributes& attr, View* presenter,
                               const std::vector<DropDownViewOption>& items, const TextViewAttributes& itemAttr)
            : ViewGroup(context, attr), presenter(*presenter) {
        addChild(presenter);

        auto* lay = new LinearLayout(context, {
                .width = WRAP_CONTENT,
                .height = WRAP_CONTENT,
                .orientation = VERTICAL,
                .gravity = LEFT
        });

        for (auto& item: items) {
            auto curItemAttr = itemAttr;
            curItemAttr.text = item.text;
            auto* itemView = new TextView(context, curItemAttr);
            itemView->setOnClickListener([this, item](View* view) {
                item.onClickListener();
                this->dialog->hide();
            });
            lay->addChild(itemView);
        }

        dialog = context->createDialog(lay, 0, 0);
    }

    DropDownView::~DropDownView() {
        delete dialog;
    }

    bool DropDownView::onClick(const MouseEvent& event) {
        if (ViewGroup::onClick(event))
            return true;
        dialog->toggleShown();
        return true;
    }

    bool DropDownView::isInside(double x, double y) {
        return presenter.isInside(x, y);
    }

    float DropDownView::getContentWidth() {
        return presenter.getContentWidth();
    }

    float DropDownView::getContentHeight() {
        return presenter.getContentWidth();
    }

    void DropDownView::onDraw() {
        presenter.draw();
    }

    SpaceRequirement DropDownView::onMeasure() {
        return presenter.onMeasure();
    }

    void DropDownView::onLayout(float left, float top, float right, float bottom) {
        presenter.layout(left, top, right, bottom);
        dialog->setX(left);
        dialog->setY(bottom);
    }
}
