//
// Created by danil on 27.10.2022.
//

#include "logging.h"
#include "OsLock.h"
#include <iomanip>
#include <chrono>
#include <mutex>

utils::OsLock print_time(utils::OsLock& os) {
    using namespace std::chrono;

    time_point<system_clock> now = high_resolution_clock::now();

    auto time = system_clock::to_time_t(now);
    tm* tm = localtime(&time);
    char timeChars[32];
    strftime(timeChars, sizeof(timeChars), "%H:%M:%S", tm);

    auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;

    /// [hh:mm:ss.mmm]
    return std::move(os << '[' << timeChars << '.' << std::setfill('0') << std::setw(3) << millis << "] ");
}

utils::OsLock info(std::ostream& os) {
    utils::OsLock osLock(os);
    return print_time(osLock);
}

void info(const std::string& s) {
    info(std::cout) << s;
}

void exceptional_exit(const std::string& msg) {
    info(std::cerr) << msg;
    throw std::exception();
}
