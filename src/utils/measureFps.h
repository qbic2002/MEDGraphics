#ifndef CPP_EXPERIMENTS_MEASUREFPS_H
#define CPP_EXPERIMENTS_MEASUREFPS_H
namespace utils {
    void initTimer();

    void updateTimer();

    float getDeltaT();

    float getCurrentTime();
}
#endif //CPP_EXPERIMENTS_MEASUREFPS_H
