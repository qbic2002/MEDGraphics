//
// Created by danil on 02.12.2022.
//

#ifndef MEDGRAPHICS_SELECT_VIEW_H
#define MEDGRAPHICS_SELECT_VIEW_H

#include "TextView.h"
#include "view/widget/LinearLayout.h"
#include "view/Dialog.h"

namespace view {

    struct SelectViewAttributes {
        VIEW_ATTRS
        TEXT_VIEW_ATTRS
        SELECT_VIEW_ATTRS
    };

    class SelectView : public TextView {
    public:
        SelectView(Context* context, const SelectViewAttributes& attr, const TextViewAttributes& itemAttr);

        ~SelectView() override;

        bool onClick(const MouseEvent& event) override;

        void onLayout(float left, float top, float right, float bottom) override;

        int getSelectIndex() const;

        void setSelectIndex(int index);

        void setOnSelectChangeListener(const std::function<void(int index)>& listener);

    protected:
        void itemSelected(int index);

        SELECT_VIEW_ATTRS
        Dialog* dialog = nullptr;
        int selectIndex = 0;
        std::function<void(int index)> onSelectChangeListener = nullptr;
    };

} // view

#endif //MEDGRAPHICS_SELECT_VIEW_H
