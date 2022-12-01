//
// Created by danil on 23.09.2022.
//

#ifndef MEDGRAPHICS_WINDOWBAR_H
#define MEDGRAPHICS_WINDOWBAR_H

#define WINDOW_BAR_HEIGHT 48

#include "view/ViewGroup.h"
#include "../core/MyApp.h"
#include "ImageView.h"
#include "view/widget/LinearLayout.h"

namespace view {

    class WindowBar : public LinearLayout {
    public:
        WindowBar(Context* context, const LinearLayoutAttributes& attr);
    };

} // view

#endif //MEDGRAPHICS_WINDOWBAR_H
