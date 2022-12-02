//
// Created by danil on 02.12.2022.
//

#include "SelectView.h"

namespace view {

    SelectView::SelectView(Context* context, const SelectViewAttributes& attr, const TextViewAttributes& itemAttr)
            : TextView(context, *(TextViewAttributes*) &attr) {
        SELECT_VIEW_ATTRS_SET(attr)

        if (!items.empty())
            text = items[0];

        auto* lay = new LinearLayout(context, {
                .width = WRAP_CONTENT,
                .height = WRAP_CONTENT,
                .orientation = VERTICAL,
                .gravity = LEFT
        });

        for (int i = 0; i < items.size(); i++) {
            auto curItemAttr = itemAttr;
            curItemAttr.text = items[i];
            auto* itemView = new TextView(context, curItemAttr);
            itemView->setOnClickListener([this, i]() {
                itemSelected(i);
                this->dialog->hide();
            });
            lay->addChild(itemView);
        }

        dialog = context->createDialog(lay, 0, 0);
    }

    SelectView::~SelectView() {
        delete dialog;
    }

    bool SelectView::onClick(const ClickEvent& event) {
        if (event.action == GLFW_RELEASE) {
            dialog->toggleShown();
            return true;
        }
        return false;
    }

    void SelectView::onLayout(float left, float top, float right, float bottom) {
        dialog->setX(left);
        dialog->setY(bottom);
    }

    int SelectView::getSelectIndex() const {
        return selectIndex;
    }

    void SelectView::setOnSelectChangeListener(const std::function<void(int)>& listener) {
        onSelectChangeListener = listener;
    }

    void SelectView::itemSelected(int index) {
        setText(items[index]);
        selectIndex = index;
        if (onSelectChangeListener) {
            onSelectChangeListener(index);
        }
    }

    void SelectView::setSelectIndex(int index) {
        itemSelected(index);
    }
} // view