//
// Created by danil on 15.12.2022.
//

#ifndef MEDGRAPHICS_IMAGE_PROCESSING_H
#define MEDGRAPHICS_IMAGE_PROCESSING_H

#include <string>
#include <vector>

namespace img {
    struct ScaleImageInfo {
        int width = 0;
        int height = 0;
        float centerShiftX = 0;
        float centerShiftY = 0;
        float* params;
    };

    struct Parameter {
        const std::wstring name;
        const float defaultValue;
    };

    struct ScaleMode {
        const std::wstring name;
        const std::vector<Parameter> params;

        void (* const scale)(const float* src, int srcWidth, int srcHeight,
                             float* dst, int dstWidth, int dstHeight,
                             int stride, float centerShiftX, float centerShiftY, float* params);
    };

    namespace scale {
        extern const ScaleMode nearest;
        extern const ScaleMode bilinear;
        extern const ScaleMode lanczos3;
        extern const ScaleMode bcSplines;
        extern const std::vector<const ScaleMode*> modes;
    }

    void histogram(const float* src, int stride, int length, int* dst, int dstLength);

    void rescale(const float* src, int stride, int length, float* dst, float leftEdge, float rightEdge);
}

#endif //MEDGRAPHICS_IMAGE_PROCESSING_H
