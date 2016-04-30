#include <cmath>
#include <iostream>

#include "particlesystem.h"
#include "particlecomputer.h"

ParticleSystem::ParticleSystem()
    : m_particles(new ParticleState)
{
    float incr = 0.03f;
    float csize = 0.5f;
    for (float x = -csize; x <= csize; x+=incr) {
        for (float y = -csize; y <= csize; y+=incr) {
            for (float z = -csize; z <= csize; z+=incr) {
                Particle p = Particle {glm::vec3(x,y,z), glm::vec3()};
                m_particles->push_back(p);
            }
        }
    }

    Force f1;
    f1.type = ForceNoise_2;
    f1.seed1 = 519;
    f1.mag1 = 0.001f;

    Force f2;
    f2.type = ForcePoint;
    f2.x = 0.0f;
    f2.y = 0.0f;
    f2.z = 3.0f;
    f2.mag1 = -0.010f;

    m_forces.push_back(f1);
    m_forces.push_back(f2);
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
    std::cout << "Number of particles: " << n_state.size() << " !" << std::endl;
    *m_particles = n_state;
}
