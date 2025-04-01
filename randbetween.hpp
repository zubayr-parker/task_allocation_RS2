#ifndef RANDBETWEEN_HPP
#define RANDBETWEEN_HPP

#include <random>

class Random {
public:
    // Generate a random integer between min and max (inclusive)
    static int randBetween(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd()); // Mersenne Twister RNG
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }
};

#endif // RANDBETWEEN_HPP
