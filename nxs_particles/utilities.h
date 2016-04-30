#ifndef UTILITIES_H
#define UTILITIES_H

#include <memory>
#include "camera.h"

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


#endif // UTILITIES_H
