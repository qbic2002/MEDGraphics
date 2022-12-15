//
// Created by danil on 15.12.2022.
//

#ifndef MEDGRAPHICS_STATIC_OBJECT_H
#define MEDGRAPHICS_STATIC_OBJECT_H

#include <iostream>

class A {
public:
    A() noexcept {
        std::cout << "A was created at " << this << std::endl;
    }

    ~A() noexcept {
        std::cout << "A was destroyed at " << this << std::endl;
    }
};

extern A a;

#endif //MEDGRAPHICS_STATIC_OBJECT_H
