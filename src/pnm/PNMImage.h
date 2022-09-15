//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PNMIMAGE_H
#define MEDGRAPHICS_PNMIMAGE_H

enum PNMMode{
    P5,
    P6
};

class PNMImage {
public:
    PNMMode pnmMode;
    int width;
    int height;
    int maxGrey;

    unsigned char* data;
};


#endif //MEDGRAPHICS_PNMIMAGE_H
