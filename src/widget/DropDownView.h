//
// Created by danil on 23.11.2022.
//

#ifndef MEDGRAPHICS_DROP_DOWN_VIEW_H
#define MEDGRAPHICS_DROP_DOWN_VIEW_H

#include <view/View.h>
#include "TextView.h"
#include <view/widget/LinearLayout.h>
#include <view/Dialog.h>

namespace view {

    class DropDownViewOption {
    public:
        DropDownViewOption(const String& text, const std::function<void()>& onClickListener = nullptr);

        String text;
        std::function<void()> onClickListener;
    };

    class DropDownView : public ViewGroup {
    public:
        DropDownView(Context* context, const ViewGroupAttributes& attr, View* presenter,
                     const std::vector<DropDownViewOption>& items, const TextViewAttributes& itemAttr);

        ~DropDownView() override;

        void onLayout(float left, float top, float right, float bottom) override;

        float getContentWidth() override;

        float getContentHeight() override;

        void onDraw() override;

        SpaceRequirement onMeasure() override;

        bool onClick(const MouseEvent& event) override;

        bool isInside(double x, double y) override;

    private:
        Dialog* dialog = nullptr;
        View& presenter;
    };
}

#endif //MEDGRAPHICS_DROP_DOWN_VIEW_H
