//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PNMHEADER_H
#define MEDGRAPHICS_PNMHEADER_H


enum PNMMode{
    P5,
    P6
};

class PNMHeader {
public:
    PNMMode pnmMode;
    int width;
    int height;
    int maxGray;
};


#endif //MEDGRAPHICS_PNMHEADER_H
