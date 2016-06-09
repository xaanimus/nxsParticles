#ifndef UTILITIES_H
#define UTILITIES_H

#include <memory>
#include <math.h>
#include "Scene/camera.h"

typedef int TimeTick;
typedef double TimeSec;

struct UpdateContainer {
    TimeTick ticks; //total ticks
    TimeTick deltaTick;
    TimeTick tick_per_sec;
    TimeSec seconds; //total seconds
};

struct DrawInfo {
    std::weak_ptr<Camera> active_camera;
};

//xorshift64star
uint64_t inline qrand_int_seed1(int seed) {
    seed ^= seed >> 12; // a
    seed ^= seed << 25; // b
    seed ^= seed >> 27; // c
    return seed * UINT64_C(2685821657736338717);
}

float inline qrand_float_seed1(float seed) {
    uint64_t r = qrand_float_seed1(*(int *)&seed);
    return *(float *)r;
}

float inline rand_float_seed(float low, float hi, float seed) {
    float f = qrand_float_seed1(seed);
    f = fmodf(f, hi - low);
    f += low;
    return f;
}

float inline rand_float(float low, float hi) {
    float f = rand() / (float) RAND_MAX;
    f *= hi - low;
    f += low;
    return f;
}

#endif // UTILITIES_H
