//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_ROOTVIEW_H
#define MEDGRAPHICS_ROOTVIEW_H

#include "../view/Context.h"
#include "../view/Style.h"
#include "../view/ViewGroup.h"

namespace view {

    class RootView : public ViewGroup {
    public:
        RootView(Context* context, const Style& style);
    };

} // view

#endif //MEDGRAPHICS_ROOTVIEW_H
