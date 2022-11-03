//
// Created by golov on 04.11.2022.
//

#include "OsLock.h"


utils::OsLock::OsLock(std::ostream& ostream)
        :
        os(ostream) {
    osLockMutex.lock();
}

utils::OsLock::~OsLock() {
    if (needToUnlock) {
        os << "\n";
        osLockMutex.unlock();
    }
}

utils::OsLock::OsLock(utils::OsLock&& other) noexcept
        :
        os(other.os) {
    other.needToUnlock = false;
}



