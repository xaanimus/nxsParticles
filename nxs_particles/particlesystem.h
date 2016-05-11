#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include "glm/glm.hpp"
#include "utilities.h"
#include "emitter.h"

class Emitter;
struct Particle;
typedef std::vector<Particle> ParticleState;

//VERY BAD TODO fix this
typedef std::vector<std::unique_ptr<Emitter> > EmitterGroup;

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

    /** seed1 : seed, mag1 : magnitude */
    ForceNoise_1,

    /** smooth noise.
        seed1 : seed, mag1 : magnitude
        x : size */
    ForceNoise_2
};

struct Force {
    float x, y, z, mag1;
    ForceType type;
    int seed1;

    const glm::vec3 xyz() const {return glm::vec3(x,y,z);}
};

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float age;
    float life;
};

struct ParticleGroup {
    std::weak_ptr<ParticleState> particles;
    int position_offset = 0, position_count = 3;
    int velocity_offset = 3, velocity_count = 3;
    int age_offset = 6, age_count = 1;
    int life_offset = 7, life_count = 1;
    int stride = sizeof(Particle);
};

/**
 * @brief The ParticleSystem class
 * NOTE: Particle systems cannot be translated/rotated/scaled.
 * A particle system is an environment that generates and manages particles.
 * It contains information about all particles, emitters, and forces in the system.
 */
class ParticleSystem
{
public:
    ParticleSystem();

    ParticleGroup getParticles();
    void update(UpdateContainer updates);
private:
    std::shared_ptr<ParticleState> m_particles;
    std::vector<Force> m_forces;
    EmitterGroup m_emitters;
};

#endif // PARTICLESYSTEM_H
