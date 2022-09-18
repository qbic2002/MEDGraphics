//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_ABSTRACTRASTER_H
#define MEDGRAPHICS_ABSTRACTRASTER_H


class AbstractRaster {
public:
    virtual int getWidth() const = 0;

    virtual int getHeight() const = 0;

    virtual const unsigned char* getRgbaData() const = 0;

    virtual ~AbstractRaster() = default;
};

#endif //MEDGRAPHICS_ABSTRACTRASTER_H
