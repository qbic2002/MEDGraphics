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



    PNMImage pnmImage = pnm::readPNMImage("assets/1.pgm");

    std::cout << pnmImage.pnmHeader.pnmMode << " " << pnmImage.pnmHeader.width << " " << pnmImage.pnmHeader.height << " " << pnmImage.pnmHeader.maxGrey << "\n";


    for (int j = 0; j < pnmImage.pnmHeader.height; ++j) {
        for (int i = 0; i < pnmImage.pnmHeader.width; ++i){
            std::cout << std::setw(3) << (int)pnmImage.rgbaData.get(i, j).R << " ";
        }
        std::cout << "\n";
    }
}