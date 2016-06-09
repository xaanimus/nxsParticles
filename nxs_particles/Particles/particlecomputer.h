#ifndef PARTICLECOMPUTER_H
#define PARTICLECOMPUTER_H

#include "utilities.h"

#include "Particles/particlesystem.h"
#include "Particles/emitter.h"

class ParticleComputer
{
public:
    ParticleComputer();

    static std::shared_ptr<ParticleComputer> get();
    virtual ParticleState interpolate(const ParticleState &particle_state,
                             const std::vector<Force> &forces,
                             TimeTick delta_tick,
                             const TimeSec &tick_per_sec,
                             EmitterGroup &emitters) = 0;
};

class CPUParticleComputer : public ParticleComputer {
public:
    CPUParticleComputer();

    virtual ParticleState interpolate(const ParticleState &particle_state,
                             const std::vector<Force> &forces,
                             TimeTick delta_tick,
                             const TimeSec &tick_per_sec,
                             EmitterGroup &emitters);

    ParticleState step(const ParticleState &particle_state,
                       const std::vector<Force> &forces,
                       const TimeSec &delta_sec,
                       EmitterGroup &emitters);
};

#endif // PARTICLECOMPUTER_H
