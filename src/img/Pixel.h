//
// Created by golov on 17.09.2022.
//

#ifndef MEDGRAPHICS_PIXEL_H
#define MEDGRAPHICS_PIXEL_H

class RGBAPixel;

class Pixel {
public:
    virtual RGBAPixel getRGBA() const = 0;

    virtual ~Pixel() = default;
};


#endif //MEDGRAPHICS_PIXEL_H
