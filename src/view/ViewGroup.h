//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_VIEWGROUP_H
#define MEDGRAPHICS_VIEWGROUP_H

#include <string>
#include "Style.h"
#include "ClickEvent.h"
#include "View.h"

namespace view {

    class ViewGroup : public View {
    public:
        ViewGroup(Context* context, const Style& style);

        ~ViewGroup() override;

        bool onClick(const ClickEvent& event) override;

        void addChild(View* view);

        void render() override;

        void onWindowResize(unsigned int width, unsigned int height) override;

        void onMouseLeave() override;

        void onMouseMove(double x, double y) override;

        bool onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

        void onMeasure(const CalculatedPos& parentPos) override;

    private:
        std::vector<View*> children;
    };

} // view

#endif //MEDGRAPHICS_VIEWGROUP_H
