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

        void setMaxValue(int maxValue);

        void setValuesR(int* values, int valueArrayLength);

        void setValuesG(int* values, int valueArrayLength);

        void setValuesB(int* values, int valueArrayLength);

        ~HistogramView() override;

    private:
        float* buildTriangleVertexArray(int barWidth, float scaleY, int canvasHeight, int* src);

        int lineWidth = 1;
        int valueArrayLength = 0;
        int maxValue;
        int* valuesR = nullptr;
        int* valuesG = nullptr;
        int* valuesB = nullptr;
    };

} // view

#endif //MEDGRAPHICS_HISTOGRAM_VIEW_H
