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
        int mode = 0;
    };

    struct ScaleMode {
        std::wstring name;

        void (* scale)(const ScaleImageInfo&);
    };

    namespace scale {
        extern const ScaleMode nearest;
//    extern const ScaleMode bilinear;
//    extern const ScaleMode lanczos3;
//    extern const ScaleMode bcSplines;
        extern const std::vector<const ScaleMode*> modes;
    }

    void histogram(const float* src, int stride, int length, int* dst, int dstLength);
}

#endif //MEDGRAPHICS_IMAGE_PROCESSING_H
