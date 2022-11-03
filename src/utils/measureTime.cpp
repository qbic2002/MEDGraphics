//
// Created by danil on 27.10.2022.
//

#include "measureTime.h"
#include "logging.h"

using namespace std::chrono;

namespace utils {
    TimeStamp::TimeStamp() {
        start = high_resolution_clock::now();
    }

    void TimeStamp::report(const std::string& label) const {
        auto time = duration_cast<milliseconds>(high_resolution_clock::now() - start);
        info() << label << " took " << time.count() << " ms";
    }

    unsigned long long TimeStamp::ms() const {
        return duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
    }
}
