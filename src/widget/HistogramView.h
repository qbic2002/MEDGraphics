//
// Created by danil on 15.12.2022.
//

#ifndef MEDGRAPHICS_HISTOGRAM_VIEW_H
#define MEDGRAPHICS_HISTOGRAM_VIEW_H

#include "view/View.h"

namespace view {

    class HistogramView : public View {
    public:
        struct ValuesColorPair {
            std::vector<int> values;
            float color[4] = {1, 1, 1, 1};
        };

        HistogramView(Context* context, const ViewAttributes& attr);

        void onDraw() override;

        void addValues(const HistogramView::ValuesColorPair& valuesColorPair);

        void reset();

        ~HistogramView() override;

    private:
        int getMaxComponent();

        float* buildTriangleVertexArray(int barWidth, float scaleY, int canvasHeight, const std::vector<int>& src);

        int lineWidth = 1;
        std::vector<ValuesColorPair> valuesColorVector;
    };

} // view

#endif //MEDGRAPHICS_HISTOGRAM_VIEW_H
