#include <ctime>
#include <iostream>

namespace utils {
    clock_t initClock;
    clock_t prevClock;
    clock_t curClock;

    float dt;
    int frames = 0;

    void initTimer() {
        initClock = clock();
        prevClock = curClock = 0;
    }

    void updateTimer() {
        curClock = clock() - initClock;
        auto elapsedTime = curClock - prevClock;
        dt = (float) elapsedTime / CLOCKS_PER_SEC;

        frames++;
        static clock_t elapsedClock = 0;
        elapsedClock += elapsedTime;
        if (elapsedClock >= CLOCKS_PER_SEC) {
            std::cout << "FPS: " << frames << std::endl;
            elapsedClock -= CLOCKS_PER_SEC;
            frames = 0;
        }

        prevClock = curClock;
    }

    float getDeltaT() {
        return dt;
    }

    float getCurrentTime() {
        return (float) curClock / CLOCKS_PER_SEC;
    }
}
