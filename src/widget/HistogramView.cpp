//
// Created by danil on 15.12.2022.
//

#include "HistogramView.h"

namespace view {
    HistogramView::HistogramView(Context* context, const ViewAttributes& attr) : View(context, attr) {}

    void HistogramView::onDraw() {
        if (valuesColorVector.empty()) {
            return;
        }
        glPushMatrix();
        {
            // View size can be got with innerEdges.width() and innerEdges.height()
            glTranslatef(innerEdges.left, innerEdges.top, 0);

            int height = innerEdges.height();
            int width = innerEdges.width();


            float scaleY = (float) height / getMaxComponent();
            int barWidth = (float) width / valuesColorVector[0].values.size();


            glLineWidth(lineWidth);
            for (const auto& valuesColorPair: valuesColorVector) {
                float* triangleVertexArray = buildTriangleVertexArray(barWidth, scaleY, height, valuesColorPair.values);

                glColor4f(valuesColorPair.color[0], valuesColorPair.color[1], valuesColorPair.color[2],
                          valuesColorPair.color[3]);
                glBegin(GL_QUADS);
                for (int i = 0; i < valuesColorPair.values.size() * 4; ++i) {
                    glVertex2f(triangleVertexArray[2 * i], triangleVertexArray[2 * i + 1]);
                }

//                glColor4f(1, 1, 1, 1);
                glEnd();

                delete[] triangleVertexArray;
            }

        }
        glPopMatrix();
    }

    HistogramView::~HistogramView() {

    }

    /// do not forget to delete result pointer
    float*
    HistogramView::buildTriangleVertexArray(int barWidth, float scaleY, int canvasHeight, const std::vector<int>& src) {
        auto* triangleVertexArray = new float[(src.size() * 4) * 2];


        int triangleVertexArrayIndex = 0;
        for (int i = 0; i < src.size(); ++i) {
            triangleVertexArray[triangleVertexArrayIndex++] = i * barWidth;
            triangleVertexArray[triangleVertexArrayIndex++] = canvasHeight;
            triangleVertexArray[triangleVertexArrayIndex++] = i * barWidth;
            triangleVertexArray[triangleVertexArrayIndex++] = canvasHeight - (src[i] * scaleY);
            triangleVertexArray[triangleVertexArrayIndex++] = (i + 1) * barWidth;
            triangleVertexArray[triangleVertexArrayIndex++] = canvasHeight - (src[i] * scaleY);
            triangleVertexArray[triangleVertexArrayIndex++] = (i + 1) * barWidth;
            triangleVertexArray[triangleVertexArrayIndex++] = canvasHeight;
        }

        return triangleVertexArray;
    }


    void HistogramView::addValues(const HistogramView::ValuesColorPair& valuesColorPair) {
        this->valuesColorVector.push_back(valuesColorPair);
    }

    int HistogramView::getMaxComponent() {
        int maxComponent = 0;
        for (const auto& item: valuesColorVector) {
            for (const auto& value: item.values) {
                if (value > maxComponent) {
                    maxComponent = value;
                }
            }
        }

        return maxComponent;
    }

    void HistogramView::reset() {
        valuesColorVector.erase(valuesColorVector.begin(), valuesColorVector.end());
    }
} // view