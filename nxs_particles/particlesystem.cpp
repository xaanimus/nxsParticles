#include "particlesystem.h"

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
}

ParticleGroup ParticleSystem::getParticles()
{
    ParticleGroup pGroup;
    pGroup.particles = m_particles;
    return pGroup;
}
