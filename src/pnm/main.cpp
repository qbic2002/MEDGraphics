//
// Created by golov on 15.09.2022.
//

#include <iostream>
#include <fstream>
#include <iomanip>

#include "utils/fileUtil.h"
#include "PNMImage.h"
#include "utils/pnmUtil.h"

int main(){

    PNMImage pnmImage = pnm::readPNMImage("assets/qbic.ppm");

    std::cout << pnmImage.pnmHeader.pnmMode << " " << pnmImage.pnmHeader.width << " " << pnmImage.pnmHeader.height
              << " " << pnmImage.pnmHeader.maxGrey << "\n";

    PNMImage p5Img = pnm::convertP6ToP5(pnmImage);
    pnm::writePNMImage(p5Img, "../../assets/cr.pnm");
}