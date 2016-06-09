#include <cmath>

#include "utilities.h"
#include "glm/gtx/rotate_vector.hpp"

#include "emitter.h"

#include <iostream>

Emitter::Emitter()
{}

PointEmitter::PointEmitter()
{}

void PointEmitter::emitParticles(std::function<void(Particle)> push_particle)
{
    for (int i = 0; i < m_rate; i++) {
        Particle p;
        p.age = 0.f;
        p.life = m_life;
        p.position = m_position;
        p.velocity = m_initial_velocity;

        if (m_emit_angle != 0) {
            float r_angle = rand_float(-m_emit_angle, m_emit_angle);
            p.velocity = glm::rotateX(p.velocity, r_angle);
            r_angle = rand_float(-m_emit_angle, m_emit_angle);
            p.velocity = glm::rotateY(p.velocity, r_angle);
        }

        push_particle(p);
    }
}
