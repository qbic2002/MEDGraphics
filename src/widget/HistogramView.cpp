//
// Created by danil on 15.12.2022.
//

#include "HistogramView.h"

namespace view {
    HistogramView::HistogramView(Context* context, const ViewAttributes& attr) : View(context, attr) {}

    void HistogramView::onDraw() {
        if (valueArrayLength == 0) {
            return;
        }
        glPushMatrix();
        {
            // View size can be got with innerEdges.width() and innerEdges.height()
            glTranslatef(innerEdges.left, innerEdges.top, 0);

            int height = innerEdges.height();
            int width = innerEdges.width();
            float scaleY = (float) height / maxValue;
            int barWidth = (float) width / valueArrayLength;


            float* triangleVertexArrayR = buildTriangleVertexArray(barWidth, scaleY, height, valuesR);
            float* triangleVertexArrayG = buildTriangleVertexArray(barWidth, scaleY, height, valuesG);
            float* triangleVertexArrayB = buildTriangleVertexArray(barWidth, scaleY, height, valuesB);

            glLineWidth(lineWidth);
            glColor4f(1, 0, 0, 1.0 / 3);
            glBegin(GL_QUADS);
            for (int i = 0; i < ((valueArrayLength * 4)); ++i) {

                glVertex2f(triangleVertexArrayR[2 * i], triangleVertexArrayR[2 * i + 1]);
            }
            glColor4f(1, 1, 1, 1);
            glEnd();

            glLineWidth(lineWidth);
            glColor4f(0, 1, 0, 1.0 / 3);
            glBegin(GL_QUADS);
            for (int i = 0; i < ((valueArrayLength * 4)); ++i) {

                glVertex2f(triangleVertexArrayG[2 * i], triangleVertexArrayG[2 * i + 1]);
            }
            glColor4f(1, 1, 1, 1);
            glEnd();

            glLineWidth(lineWidth);
            glColor4f(0, 0, 1, 1.0 / 3);
            glBegin(GL_QUADS);
            for (int i = 0; i < ((valueArrayLength * 4)); ++i) {

                glVertex2f(triangleVertexArrayB[2 * i], triangleVertexArrayB[2 * i + 1]);
            }
            glColor4f(1, 1, 1, 1);
            glEnd();

            delete[] triangleVertexArrayR;
            delete[] triangleVertexArrayG;
            delete[] triangleVertexArrayB;
        }
        glPopMatrix();
    }

    void HistogramView::changeLineWidth(int diff) {
//        lineWidth += diff;
//        if (lineWidth < 1)
//            lineWidth = 1;
//        if (lineWidth > 10)
//            lineWidth = 10;
//        invalidate();
    }

    HistogramView::~HistogramView() {
        delete[] valuesR;
        delete[] valuesG;
        delete[] valuesB;
    }

    /// do not forget to delete result pointer
    float* HistogramView::buildTriangleVertexArray(int barWidth, float scaleY, int canvasHeight, int* src) {
        auto* triangleVertexArray = new float[(valueArrayLength * 4) * 2];


        int triangleVertexArrayIndex = 0;
        for (int i = 0; i < valueArrayLength; ++i) {
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


    void HistogramView::setMaxValue(int maxValue) {
        HistogramView::maxValue = maxValue;
    }

    void HistogramView::setValuesR(int* values, int valueArrayLength) {
        this->valueArrayLength = valueArrayLength;

        delete[] this->valuesR;
        this->valuesR = new int[valueArrayLength];
        memcpy(this->valuesR, values, valueArrayLength * sizeof(int));
    }

    void HistogramView::setValuesG(int* values, int valueArrayLength) {
        this->valueArrayLength = valueArrayLength;

        delete[] this->valuesG;
        this->valuesG = new int[valueArrayLength];
        memcpy(this->valuesG, values, valueArrayLength * sizeof(int));
    }

    void HistogramView::setValuesB(int* values, int valueArrayLength) {
        this->valueArrayLength = valueArrayLength;

        delete[] this->valuesB;
        this->valuesB = new int[valueArrayLength];
        memcpy(this->valuesB, values, valueArrayLength * sizeof(int));
    }
} // view