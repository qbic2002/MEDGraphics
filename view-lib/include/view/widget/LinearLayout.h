//
// Created by danil on 04.11.2022.
//

#ifndef MEDGRAPHICS_LINEARLAYOUT_H
#define MEDGRAPHICS_LINEARLAYOUT_H

#include "view/ViewGroup.h"

namespace view {

    enum Orientation {
        VERTICAL,
        HORIZONTAL
    };

    class LinearLayout : public ViewGroup {
    public:
        LinearLayout(Context* context, const Style& style, Orientation orientation = VERTICAL) :
                ViewGroup(context, style),
                orientation(orientation) {
        }

        space_requirement howMuchSpaceRequired() override {
            float childrenSize[2] = {0, 0};
            int oIndex = orientation == VERTICAL ? 1 : 0; // orientation index
            float max_size = 0;
            for (auto* child: children) {
                auto req = child->howMuchSpaceRequired();
                if (max_size < req.size[1 - oIndex])
                    max_size = req.size[1 - oIndex];
                childrenSize[oIndex] += req.size[oIndex];
            }
            childrenSize[1 - oIndex] = max_size;

            float width = styleState->padding.width() + styleState->border.width()
                          + styleState->width.pixel
                          + styleState->width.contentK * childrenSize[0];
            float height = styleState->padding.top + styleState->padding.bottom
                           + styleState->border.top + styleState->border.bottom
                           + styleState->height.pixel
                           + styleState->height.contentK * childrenSize[1];

            return {
                    .width = width,
                    .height = height,
                    .parentPartW = styleState->width.parentK,
                    .parentPartH = styleState->height.parentK,
                    .parentSparePartW = styleState->width.parentSpareK,
                    .parentSparePartH = styleState->height.parentSpareK};
        }

        void useThisSpace(float left, float top, float right, float bottom) override {
            View::useThisSpace(left, top, right, bottom);

            float inner[4] = {left + styleState->padding.left + styleState->border.left,
                              top + styleState->padding.top + styleState->border.top,
                              right - styleState->padding.right - styleState->border.right,
                              bottom - styleState->padding.bottom - styleState->border.bottom};

            float innerSize[2] = {inner[2] - inner[0], inner[3] - inner[1]};
            int oIndex = orientation == VERTICAL ? 1 : 0; // orientation index

            float spareSpace = innerSize[oIndex];
            float spareWeight = 0;
            for (auto* child: children) {
                auto req = child->howMuchSpaceRequired();
                spareSpace -= req.size[oIndex] + req.parentPart[oIndex] * innerSize[oIndex];
                spareWeight += req.parentSparePart[oIndex];
            }
            if (spareWeight == 0)
                spareWeight = 1; // avoid division by zero
            if (spareSpace < 0)
                spareSpace = 0;
            for (auto* child: children) {
                auto req = child->howMuchSpaceRequired();

                float childEdges[2];
                for (int i = 0; i < 2; i++)
                    childEdges[i] = inner[i] + req.size[i] + req.parentPart[i] * innerSize[i];

                // add spare space on orientation line
                childEdges[oIndex] += req.parentSparePart[oIndex] / spareWeight * spareSpace;
                // add all spare space to non-orientation line
                if (childEdges[1 - oIndex] >= inner[3 - oIndex] || req.parentSparePart[1 - oIndex] > 0) {
                    childEdges[1 - oIndex] = inner[3 - oIndex];
                }

                child->useThisSpace(inner[0], inner[1], childEdges[0], childEdges[1]);
                inner[oIndex] = childEdges[oIndex];
            }
        }

        Orientation orientation;
    };

} // view

#endif //MEDGRAPHICS_LINEARLAYOUT_H
