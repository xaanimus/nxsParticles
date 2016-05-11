#ifndef EMITTER_H
#define EMITTER_H

#include <functional>
#include <memory>
#include "glm/glm.hpp"
#include "particlesystem.h"

struct Particle;
class Emitter;
typedef std::vector<std::unique_ptr<Emitter> > EmitterGroup;

/**
 * @brief Emits particles into a particle system
 * An Emitter is in charge of emiting particles into a particle system.
 * Particles are emitted each time Emitter::emitParticles(f) is called,
 * where f is the function that adds a particle to the particle system.
 */
class Emitter
{
public:
    Emitter();
    virtual void emitParticles(std::function<void(Particle)> push_particle) = 0;
};

class PointEmitter : public Emitter {
public:
    PointEmitter();
    virtual void emitParticles(std::function<void(Particle)> push_particle);

    glm::vec3 m_position;
    glm::vec3 m_initial_velocity;
    float m_emit_angle;
    int m_rate;
    float m_life;
    int m_seed;
};

#endif // EMITTER_H
