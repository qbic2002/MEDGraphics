//
// Created by danil on 27.10.2022.
//

#ifndef MEDGRAPHICS_MEASURE_TIME_H
#define MEDGRAPHICS_MEASURE_TIME_H

#include <chrono>
#include <string>

namespace utils {
    class TimeStamp {
    public:
        TimeStamp();

        void report(const std::string& label) const;

        unsigned long long ms() const;

    private:
        std::chrono::time_point<std::chrono::system_clock> start;
    };
}

#endif //MEDGRAPHICS_MEASURE_TIME_H
