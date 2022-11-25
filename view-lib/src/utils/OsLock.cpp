//
// Created by golov on 04.11.2022.
//

#include "utils/OsLock.h"

namespace utils {
    OsLock::OsLock(std::ostream& ostream) : os(ostream) {
        osLockMutex.lock();
    }

    OsLock::~OsLock() {
        if (needToUnlock) {
            os << std::endl;
            osLockMutex.unlock();
        }
    }

    OsLock::OsLock(OsLock&& other) noexcept: os(other.os) {
        other.needToUnlock = false;
    }
}
