//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_WINDOWBAR_H
#define MEDGRAPHICS_WINDOWBAR_H

#include "../view/ViewGroup.h"

namespace view {

    class WindowBar : public ViewGroup {
    public:
        WindowBar(Context* context, const Style& style);
    };

} // view

#endif //MEDGRAPHICS_WINDOWBAR_H
