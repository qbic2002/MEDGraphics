//
// Created by danil on 23.09.2022.
//

#include "RootView.h"
#include "BgRenderer.h"
#include "ImageView.h"
#include "WindowBar.h"

namespace view {
    RootView::RootView(MyApp* context, const LinearLayoutAttributes& attr) : LinearLayout(context, attr) {
//        addChild(new BgRenderer(context));
        addChild(new WindowBar(context, {
                .width = FILL_PARENT,
                .height = WINDOW_BAR_HEIGHT,
                .background = ColorBackground{rgba{COLOR_WINDOW_BAR_BG}},
                .orientation = HORIZONTAL,
                .gravity = VCENTER}));
        addChild(new ImageView(context, {
                .id = IMAGE_VIEW_ID,
                .width = FILL_PARENT,
                .height = FILL_SPARE,
                .style= Style{true}}));
    }
} // view