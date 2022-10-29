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
        explicit DropDownViewElementPrototype(const String& text,
                                              const std::function<void()>& onClickListener = nullptr);

        String text;
        std::function<void()> onClickListener;
    };

    class DropDownViewElement : public TextView {
    public:
        DropDownViewElement(Context* context,
                            const Style& style,
                            const String& text);
    };

    class DropDownView : public ViewGroup {
    public:
        DropDownView(Context* context,
                     const Style& style,
                     const std::vector<DropDownViewElementPrototype>& prototypes,
                     const Style& elementsStyle);

        void onMeasure(const view::CalculatedPos& parentPos) override;

        bool isInside(double x, double y) override;

        void render() override;

        void toggleOpened();

    protected:
        bool isOpened = false;
    };

} // view

#endif //MEDGRAPHICS_DROPDOWNVIEW_H
