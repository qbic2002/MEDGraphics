//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RGBAPIXEL_H
#define MEDGRAPHICS_RGBAPIXEL_H

//#pragma pack(1)
class RGBAPixel {
public:
    RGBAPixel() = default;
    RGBAPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : R(r), G(g), B(b), A(a) {}

public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};
//#pragma pack()


#endif //MEDGRAPHICS_RGBAPIXEL_H
