#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include "glm/glm.hpp"
#include "utilities.h"

struct Particle;
typedef std::vector<Particle> ParticleState;

/**
 * parameters for the force are written in comments
 * units are standard SI (kg,m,s)
*/
enum ForceType {
    /** (x,y,z): location; mag1: attraction constant (a = Gm/r^2 = mag1/r^2) */
    ForcePoint,

    /** (x,y,z): direction unit vector; mag1: acceleration
    i.e. for gravity (x,y,z) = (0,0,-1); mag1 = 9.8 */
    ForceDirectional,

    /** TODO spec */
    ForceNoise_1
};

struct Force {
    float x, y, z, mag1;
    ForceType type;

    const glm::vec3 xyz() const {return glm::vec3(x,y,z);}
};

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
    void update(UpdateContainer updates);
private:
    std::shared_ptr<ParticleState> m_particles;
    std::vector<Force> m_forces;
};

#endif // PARTICLESYSTEM_H
