//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_RGBA_H
#define MEDGRAPHICS_RGBA_H

struct rgba {
    unsigned char r{}, g{}, b{}, a{};
};

inline unsigned char to255(float v) {
    return (unsigned char) (v * 255);
}

inline rgba to255rgba(float r, float g, float b, float a) {
    return {to255(r), to255(g), to255(b), to255(a)};
}

#endif //MEDGRAPHICS_RGBA_H
