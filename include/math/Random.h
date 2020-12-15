#pragma once

#include <ctime>
#include <random>

static bool TimeInitialized = false;
class Random
{
    static void InitTime(bool useSeed = true) {
        std::srand(useSeed ? (int)time(0) : 2);
        TimeInitialized = true;
    }

public:
    static double Get(double l = 0.0, double r = 1.0) {
        if(!TimeInitialized) InitTime(false);

        std::random_device rd;                       // create object for seeding
        std::mt19937 gen{ rd() };                    // create engine and seed it
        std::uniform_real_distribution<> dist(l, r); // create distribution for integers with [1; 9] range
        return dist(gen);
    }
};