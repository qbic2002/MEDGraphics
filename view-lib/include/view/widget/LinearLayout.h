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

    enum Gravity {
        LEFT, TOP, RIGHT, BOTTOM, HCENTER, VCENTER, CENTER
    };

    struct LinearLayoutAttributes {
        VIEW_ATTRS
        VIEW_GROUP_ATTRS
        LINEAR_LAYOUT_ATTRS
    };

    class LinearLayout : public ViewGroup {
    public:
        LinearLayout(Context* context, const LinearLayoutAttributes& attr)
                : ViewGroup(context, (const ViewGroupAttributes&) attr) {
            LINEAR_LAYOUT_ATTRS_SET(attr)
        }

        SpaceRequirement onMeasure() override {
            float childrenSize[2] = {0, 0};
            int oIndex = orientation == VERTICAL ? 1 : 0; // orientation index
            float max_size = 0;
            for (auto* child: children) {
                if (child->getVisibility() == INVISIBLE)
                    continue;
                auto req = child->measure();
                if (max_size < req.size[1 - oIndex])
                    max_size = req.size[1 - oIndex];
                childrenSize[oIndex] += req.size[oIndex];
            }
            childrenSize[1 - oIndex] = max_size;

            return {
                    .width = width.evaluateContent(childrenSize[0]) + padding.width(),
                    .height = height.evaluateContent(childrenSize[1]) + padding.height(),
                    .parentPartW = width.parentK,
                    .parentPartH = height.parentK,
                    .parentSparePartW = width.parentSpareK,
                    .parentSparePartH = height.parentSpareK};
        }

        void onLayout(float left, float top, float right, float bottom) override {
            float inner[4] = {innerEdges.left, innerEdges.top, innerEdges.right, innerEdges.bottom};

            float innerSize[2] = {inner[2] - inner[0], inner[3] - inner[1]};
            int oIndex = orientation == HORIZONTAL ? 0 : 1; // orientation index

            float spareSpace = innerSize[oIndex];
            float spareWeight = 0;
            for (auto* child: children) {
                if (child->getVisibility() == INVISIBLE)
                    continue;
                auto req = child->measure();
                spareSpace -= req.size[oIndex] + req.parentPart[oIndex] * innerSize[oIndex];
                spareWeight += req.parentSparePart[oIndex];
            }
            if (spareSpace < 0)
                spareSpace = 0;
            if (spareWeight == 0) {
                spareWeight = 1; // avoid division by zero
                if (gravity == CENTER || gravity == HCENTER + oIndex) {
                    inner[oIndex] += spareSpace / 2;
                } else if (gravity == RIGHT + oIndex) {
                    inner[oIndex] += spareSpace;
                }
            }
            float nonOrientGravityK = 0;
            if (gravity == CENTER || gravity == HCENTER + 1 - oIndex) {
                nonOrientGravityK = 0.5;
            } else if (gravity == RIGHT + 1 - oIndex) {
                nonOrientGravityK = 1;
            }
            for (auto* child: children) {
                auto req = child->measure();
                if (child->getVisibility() == INVISIBLE)
                    continue;

                float childEdges[2];
                for (int i = 0; i < 2; i++)
                    childEdges[i] = inner[i] + req.size[i] + req.parentPart[i] * innerSize[i];

                float gravityShift = 0;

                // add spare space on orientation line
                childEdges[oIndex] += req.parentSparePart[oIndex] / spareWeight * spareSpace;
                // add all spare space to non-orientation line
                if (childEdges[1 - oIndex] >= inner[3 - oIndex] || req.parentSparePart[1 - oIndex] > 0) {
                    childEdges[1 - oIndex] = inner[3 - oIndex];
                } else {
                    gravityShift = nonOrientGravityK * (inner[3 - oIndex] - childEdges[1 - oIndex]);
                    childEdges[1 - oIndex] += gravityShift;
                    inner[1 - oIndex] += gravityShift;
                }

                child->layout(inner[0], inner[1], childEdges[0], childEdges[1]);
                inner[1 - oIndex] -= gravityShift;
                inner[oIndex] = childEdges[oIndex];
            }
        }

    protected:
        LINEAR_LAYOUT_ATTRS
    };

} // view

#endif //MEDGRAPHICS_LINEARLAYOUT_H
