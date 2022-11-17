#ifndef CPP_EXPERIMENTS_MEASURE_FPS_H
#define CPP_EXPERIMENTS_MEASURE_FPS_H

namespace utils {
    void initTimer();

    void updateTimer();

    float getDeltaT();

    float getCurrentTime();
}
#endif //CPP_EXPERIMENTS_MEASURE_FPS_H
