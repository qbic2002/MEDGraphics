//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_ROOTVIEW_H
#define MEDGRAPHICS_ROOTVIEW_H

#include "../core/MyApp.h"
#include "view/Style.h"
#include "view/ViewGroup.h"

namespace view {

    class RootView : public ViewGroup {
    public:
        RootView(MyApp* context, const Style& style);
    };

} // view

#endif //MEDGRAPHICS_ROOTVIEW_H
