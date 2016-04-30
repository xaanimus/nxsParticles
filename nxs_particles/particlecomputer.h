#ifndef PARTICLECOMPUTER_H
#define PARTICLECOMPUTER_H

#include "particlesystem.h"
#include "utilities.h"

class ParticleComputer
{
public:
    ParticleComputer();

    static std::shared_ptr<ParticleComputer> get();
    virtual ParticleState interpolate(const ParticleState &particle_state,
                             const std::vector<Force> &forces,
                             TimeTick delta_tick,
                             const TimeSec &tick_per_sec) = 0;
};

class CPUParticleComputer : public ParticleComputer {
public:
    CPUParticleComputer();

    virtual ParticleState interpolate(const ParticleState &particle_state,
                             const std::vector<Force> &forces,
                             TimeTick delta_tick,
                             const TimeSec &tick_per_sec);

    ParticleState step(const ParticleState &particle_state,
                       const std::vector<Force> &forces,
                       const TimeSec &delta_sec);
};

#endif // PARTICLECOMPUTER_H
