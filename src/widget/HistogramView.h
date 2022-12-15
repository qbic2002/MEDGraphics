//
// Created by danil on 15.12.2022.
//

#ifndef MEDGRAPHICS_HISTOGRAM_VIEW_H
#define MEDGRAPHICS_HISTOGRAM_VIEW_H

#include "view/View.h"

namespace view {

    class HistogramView : public View {
    public:
        HistogramView(Context* context, const ViewAttributes& attr);

        void onDraw() override;

        void changeLineWidth(int diff);

    private:
        int lineWidth = 1;
    };

} // view

#endif //MEDGRAPHICS_HISTOGRAM_VIEW_H
