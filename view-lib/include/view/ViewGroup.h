//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_VIEWGROUP_H
#define MEDGRAPHICS_VIEWGROUP_H

#include <string>
#include "Style.h"
#include "MouseEvent.h"
#include "View.h"

namespace view {

    struct ViewGroupAttributes {
        VIEW_ATTRS
        VIEW_GROUP_ATTRS
    };

    class ViewGroup : public View {
    public:
        ViewGroup(Context* context, const ViewGroupAttributes& attr);

        ~ViewGroup() override;

        bool onClick(const MouseEvent& event) override;

        bool onDrag(double x, double y, double dx, double dy) override;

        void addChild(View* view);

        void onDraw() override;

        void onWindowResize(unsigned int width, unsigned int height) override;

        void onMouseLeave() override;

        void onMouseMove(double x, double y) override;

        bool onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

        View* findViewById(int id) override;

        bool onKey(int key, int scancode, int action, int mods) override;

        bool onChar(unsigned int codepoint) override;

        bool onMouseEvent(const MouseEvent& event) override;

    protected:
        VIEW_GROUP_ATTRS
    };

} // view

#endif //MEDGRAPHICS_VIEWGROUP_H
