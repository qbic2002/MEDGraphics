//
// Created by danil on 27.10.2022.
//

#include "logging.h"
#include <iomanip>
#include <chrono>
#include <mutex>

std::ostream& print_time(std::ostream& os) {
    using namespace std::chrono;

    time_point<system_clock> now = high_resolution_clock::now();

    auto time = system_clock::to_time_t(now);
    tm* tm = localtime(&time);
    char timeChars[32];
    strftime(timeChars, sizeof(timeChars), "%H:%M:%S", tm);

    auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;

    /// [hh:mm:ss.mmm]
    return os << '[' << timeChars << '.' << std::setfill('0') << std::setw(3) << millis << "] ";
}

std::ostream& info() {
    return print_time(std::cout);
}

void info(const std::string& s) {
    print_time(std::cout) << s << std::endl;
}

void exceptional_exit(const std::string& msg) {
    print_time(std::cerr) << msg << std::endl;
    throw std::exception();
}
