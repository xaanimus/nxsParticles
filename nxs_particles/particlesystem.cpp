#include "particlesystem.h"
#include "particlecomputer.h"

ParticleSystem::ParticleSystem()
    : m_particles(new ParticleState)
{
    m_particles->push_back(Particle { glm::vec3(-.5f, -.5f,  0.0f), glm::vec3() });
    m_particles->push_back(Particle { glm::vec3( .5f, -.5f,  0.0f), glm::vec3() });
    m_particles->push_back(Particle { glm::vec3( .5f,  .5f,  0.0f), glm::vec3() });
    m_particles->push_back(Particle { glm::vec3(-.5f,  .5f,  0.0f), glm::vec3() });
    m_particles->push_back(Particle { glm::vec3(-.5f, -.5f,  0.5f), glm::vec3() });
    m_particles->push_back(Particle { glm::vec3( .5f, -.5f,  0.5f), glm::vec3() });
    m_particles->push_back(Particle { glm::vec3( .5f,  .5f,  0.5f), glm::vec3() });
    m_particles->push_back(Particle { glm::vec3(-.5f,  .5f,  0.5f), glm::vec3() });

    m_forces.push_back(Force {0.f, 0.f, 0.f, -0.001f, ForcePoint});
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
                                         updates.deltaTick, updates.tick_per_sec);
    *m_particles = n_state;
}
