//
// Created by golov on 04.11.2022.
//

#ifndef MEDGRAPHICS_OSLOCK_H
#define MEDGRAPHICS_OSLOCK_H

#include <mutex>
#include <iostream>

namespace utils {

    static std::mutex osLockMutex;

    class OsLock {
    public:
        explicit OsLock(std::ostream& ostream = std::cout);

        OsLock(const OsLock& other) = delete;

        OsLock(OsLock&& other) noexcept;

        ~OsLock();

        template<class T>
        OsLock& operator<<(const T& x) {
            os << x;
            return *this;
        }

    private:
        std::ostream& os;
        bool needToUnlock = true;
    };
}


#endif //MEDGRAPHICS_OSLOCK_H
