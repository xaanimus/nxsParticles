#include <cmath>
#include <iostream>

#include "particlesystem.h"
#include "particlecomputer.h"

ParticleSystem::ParticleSystem()
    : m_particles(new ParticleState)
{

    Force f1;
    f1.type = ForceNoise_2;
    f1.seed1 = 253;
    f1.mag1 = 0.0300f;
    f1.x = .1f;

    Force f2;
    f2.type = ForcePoint;
    f2.x = 1.5f;
    f2.y = 1.5f;
    f2.z = 1.5f;
    f2.mag1 = -.800f;

    m_forces.push_back(f1);
    m_forces.push_back(f2);

    PointEmitter e1;
    e1.m_emit_angle = M_PI / 10;
    e1.m_rate = 100;
    e1.m_initial_velocity = glm::vec3(0.1f,0.1f,0.1f);
    e1.m_seed = 1565;
    e1.m_life = .2f;
    e1.m_position = glm::vec3(.0f, .0f, .0f);

    m_emitters.push_back(std::unique_ptr<Emitter>(new PointEmitter(e1)));
}

ParticleGroup ParticleSystem::getParticles()
{
    ParticleGroup pGroup;
    pGroup.particles = m_particles;
    return pGroup;
}

void ParticleSystem::update(UpdateContainer updates)
{   
    ParticleState n_state = ParticleComputer::get()->interpolate(*m_particles, m_forces,
                                         updates.deltaTick, updates.tick_per_sec, m_emitters);
    std::cout << "Number of particles: " << n_state.size() << " !" << std::endl;
    *m_particles = n_state;
}
