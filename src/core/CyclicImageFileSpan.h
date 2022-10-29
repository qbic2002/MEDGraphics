//
// Created by danil on 29.10.2022.
//

#ifndef MEDGRAPHICS_CYCLIC_IMAGE_FILE_SPAN_H
#define MEDGRAPHICS_CYCLIC_IMAGE_FILE_SPAN_H

#include <set>
#include <condition_variable>
#include <thread>
#include "ImageFile.h"

class CyclicVectorIterator : public std::iterator<std::forward_iterator_tag, ImageFile> {
public:
    explicit CyclicVectorIterator(std::vector<ImageFile>& container, int index);

    ImageFile& operator*();

    bool operator<(const CyclicVectorIterator& other) const;

    bool operator>(const CyclicVectorIterator& other) const;

    bool operator==(const CyclicVectorIterator& other) const;

    bool operator!=(const CyclicVectorIterator& other) const;

    CyclicVectorIterator& operator++();

    int getIndex() const;

private:
    std::vector<ImageFile>& container;
    int index;
};

class CyclicImageFileSpan {
public:
    CyclicImageFileSpan(const CyclicVectorIterator& beginItr, const CyclicVectorIterator& endItr);

    CyclicVectorIterator begin();

    CyclicVectorIterator end();

private:
    CyclicVectorIterator beginItr;
    CyclicVectorIterator endItr;
};

#endif //MEDGRAPHICS_CYCLIC_IMAGE_FILE_SPAN_H
