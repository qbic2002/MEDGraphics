//
// Created by danil on 04.11.2022.
//

#ifndef MEDGRAPHICS_LINEARLAYOUT_H
#define MEDGRAPHICS_LINEARLAYOUT_H

#include "view/ViewGroup.h"

namespace view {

    enum Orientation {
        VERTICAL,
        HORIZONTAL
    };

    class LinearLayout : public ViewGroup {
    public:
        LinearLayout(Context* context, const Style& style, Orientation orientation = VERTICAL) :
                ViewGroup(context, style),
                orientation(orientation) {
        }

        Orientation orientation;
    };

} // view

#endif //MEDGRAPHICS_LINEARLAYOUT_H
