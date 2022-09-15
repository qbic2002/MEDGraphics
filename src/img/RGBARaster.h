//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RGBARASTER_H
#define MEDGRAPHICS_RGBARASTER_H


class RGBARaster {
public:
    RGBARaster() = default;
    RGBARaster(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : R(r), G(g), B(b), A(a) {}

public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};


#endif //MEDGRAPHICS_RGBARASTER_H
