#include <random>
#include "random.h"

namespace utils {
    unsigned int random() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
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
}