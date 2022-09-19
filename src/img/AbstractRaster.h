//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_ABSTRACTRASTER_H
#define MEDGRAPHICS_ABSTRACTRASTER_H


#include "PixelType.h"

class AbstractRaster {
public:
    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;

    virtual const unsigned char* getRgbaData() const = 0;

    virtual PixelType getPixelType() const = 0;

//    virtual Raster<RGBAPixel> compress(int width, int height) = 0;

    virtual ~AbstractRaster() = default;
};

#endif //MEDGRAPHICS_ABSTRACTRASTER_H
