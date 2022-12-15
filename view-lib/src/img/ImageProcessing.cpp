//
// Created by danil on 15.12.2022.
//

#include "img/ImageProcessing.h"

namespace img::scale {
    void scaleNearest(const ScaleImageInfo& info) {

    }

    const ScaleMode nearest{L"Ближайший", scaleNearest};

    const std::vector<const ScaleMode*> modes = {&nearest};
}

void img::histogram(const float* src, int stride, int length, int* dst, int dstLength, int& topValuesCount) {
    for (int i = 0; i < dstLength; i++) {
        dst[i] = 0;
    }
    for (int i = 0; i < length; i++) {
        int index = (int) (*src * dstLength);
        if (index == dstLength)
            index = dstLength - 1;
        ++dst[index];
        src += stride;
    }
    dst[0] = 0;
    topValuesCount = 0;
    for (int i = 0; i < dstLength; i++) {
        if (dst[i] > topValuesCount)
            topValuesCount = dst[i];
    }
}
