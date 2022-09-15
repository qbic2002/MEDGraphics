#include <ctime>
#include <iostream>

namespace utils {
    clock_t prevT;
    float dt;
    int frames = 0;

    void initTimer() {
        prevT = clock();
    }

    void updateTimer() {
        auto curT = clock();
        auto elapsedTime = curT - prevT;
        dt = (float) elapsedTime / CLOCKS_PER_SEC;

        frames++;
        static clock_t elapsedClock = 0;
        elapsedClock += elapsedTime;
        if (elapsedClock >= CLOCKS_PER_SEC) {
            std::cout << "FPS: " << frames << std::endl;
            elapsedClock -= CLOCKS_PER_SEC;
            frames = 0;
        }

        prevT = curT;
    }

    float getDeltaT() {
        return dt;
    }
}
