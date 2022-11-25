//
// Created by danil on 25.11.2022.
//

#include "ColorModels.h"

ColorModelRGB colorModelRgb;
ColorModelRGBA colorModelRgba;
ColorModelGray colorModelGray;
ColorModelHSL colorModelHsl;

ColorModel* colorModels[4] = {&colorModelRgb, &colorModelRgba, &colorModelGray, &colorModelHsl};

const ColorModel* findColorModel(ColorModelEnum colorModelEnum) {
    return colorModels[colorModelEnum];
}
