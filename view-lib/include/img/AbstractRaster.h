//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_ABSTRACTRASTER_H
#define MEDGRAPHICS_ABSTRACTRASTER_H


#include "PixelType.h"

template<class T>
concept CheckPixelType = std::is_base_of<Pixel, T>::value;

template<class T> requires CheckPixelType<T>
class Raster;

class AbstractRaster {
public:
    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;

    virtual const unsigned char* getRgbaData() const = 0;

    virtual PixelType getPixelType() const = 0;

    virtual AbstractRaster* clone() const = 0;

    virtual ~AbstractRaster() = default;
};

#endif //MEDGRAPHICS_ABSTRACTRASTER_H