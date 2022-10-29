//
// Created by danil on 29.10.2022.
//

#include "CyclicImageFileSpan.h"

CyclicVectorIterator::CyclicVectorIterator(std::vector<ImageFile>& container, int index) : container(container),
                                                                                           index(index) {}

ImageFile& CyclicVectorIterator::operator*() {
    int tmp = (index + container.size()) % container.size();
    return container[tmp];
}

bool CyclicVectorIterator::operator<(const CyclicVectorIterator& other) const {
    return !container.empty() && index < other.index;
}

bool CyclicVectorIterator::operator>(const CyclicVectorIterator& other) const {
    return !container.empty() && index > other.index;
}

bool CyclicVectorIterator::operator==(const CyclicVectorIterator& other) const {
    return container.empty() || index == other.index;
}

bool CyclicVectorIterator::operator!=(const CyclicVectorIterator& other) const {
    return !container.empty() && index != other.index;
}

CyclicVectorIterator& CyclicVectorIterator::operator++() {
    ++index;
    return *this;
}

int CyclicVectorIterator::getIndex() const {
    return index;
}

CyclicImageFileSpan::CyclicImageFileSpan(const CyclicVectorIterator& beginItr, const CyclicVectorIterator& endItr)
        : beginItr(beginItr), endItr(endItr) {}

CyclicVectorIterator CyclicImageFileSpan::begin() {
    return beginItr;
}

CyclicVectorIterator CyclicImageFileSpan::end() {
    return endItr;
}
