//
// Created by danil on 15.12.2022.
//

#include "HistogramView.h"

namespace view {
    HistogramView::HistogramView(Context* context, const ViewAttributes& attr) : View(context, attr) {}

    void HistogramView::onDraw() {
        glPushMatrix();
        {
            // View size can be got with innerEdges.width() and innerEdges.height()
            glTranslatef(innerEdges.left, innerEdges.top, 0);

            glLineWidth(lineWidth);

            glBegin(GL_LINE_STRIP);
            glVertex2f(0, 0);
            glVertex2f(20, 20);
            glVertex2f(0, 40);
            glEnd();
        }
        glPopMatrix();
    }

    void HistogramView::changeLineWidth(int diff) {
        lineWidth += diff;
        if (lineWidth < 1)
            lineWidth = 1;
        if (lineWidth > 10)
            lineWidth = 10;
        invalidate();
    }
} // view