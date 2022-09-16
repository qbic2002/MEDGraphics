#include <random>
#include "random.h"

namespace utils {
    std::uniform_real_distribution<> uniform(0, 1);
    std::random_device rd;
    std::mt19937 gen(rd());

    unsigned int random() {
        return gen();
    }

    unsigned int random(unsigned int max) {
        return random() % max;
    }

    int random(int min, int max) {
        int range = max - min;
        if (range <= 0)
            return min;
        return min + (int) (random() % range);
    }

    float randomFloat() {
        return uniform(gen);
    }

    float randomFloat(float min, float max) {
        return randomFloat() * (max - min) + min;
    }
}