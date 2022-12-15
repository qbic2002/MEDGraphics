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