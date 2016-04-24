#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include "glm/glm.hpp"

struct Particle;
typedef std::vector<Particle> ParticleState;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
};

struct ParticleGroup {
    std::weak_ptr<ParticleState> particles;
    int position_offset = 0, position_count = 3;
    int velocity_offset = 3, velocity_count = 3;
    int stride = 2 * sizeof(glm::vec3);
};

class ParticleSystem
{
public:
    ParticleSystem();

    ParticleGroup getParticles();
private:
    std::shared_ptr<ParticleState> m_particles;
};

#endif // PARTICLESYSTEM_H
