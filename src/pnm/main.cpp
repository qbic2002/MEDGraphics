//
// Created by golov on 15.09.2022.
//

#include <iostream>

#include "PNMImage.h"
#include "pnmUtils.h"

int main() {
    std::cout << sizeof(PixelRGBA8) << " " << sizeof(Pixel) << "\n";

    Raster<PixelRGBA8> raster(10, 10);
    PNMImage pnmImage = pnm::readPnmImage("assets/qbic.ppm");

    std::cout << pnmImage.pnmHeader.pnmMode << " " << pnmImage.pnmHeader.width << " " << pnmImage.pnmHeader.height
              << " " << pnmImage.pnmHeader.maxGray << "\n";

    PNMImage p5Img = pnm::convertP6ToP5(pnmImage);
    pnm::writePNMImage(p5Img, "assets/cr.pnm");
}