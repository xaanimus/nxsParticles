#include <iostream>
#include "Particles/particlecomputer.h"
#include "glm/gtx/norm.hpp"

std::shared_ptr<ParticleComputer> particle_computer_instance;

/** calculates the acceleration vector on particle for a point force */
glm::vec3 acc_from_point_force(glm::vec3 force_pos, glm::vec3 part_pos, float mag1)
{
    glm::vec3 r = force_pos - part_pos;
    float r_mag = glm::l2Norm(r);
    return mag1 * r / (r_mag * r_mag);
}

/** calculates the acceleration vector on particle for a directional force */
glm::vec3 acc_from_directional_force(glm::vec3 direction_unit, float mag1)
{
    return direction_unit * mag1;
}

int mod_factor = 10000;
glm::vec3 acc_from_noise(glm::vec3 part_pos, int seed, float mag1)
{
    int x = *(int *)&part_pos.x;
    int y = *(int *)&part_pos.y;
    int z = *(int *)&part_pos.z;

    uint32_t t = seed;
    int resx = t ^ (x << 3) ^ (y << 6) ^ (z << 9);
    int resy = t ^ (x << 5) ^ (y << 10) ^ (z << 15);
    int resz = t ^ (x << 8) ^ (y << 17) ^ (z << 25);

    resx %= mod_factor;
    resy %= mod_factor;
    resz %= mod_factor;

    return mag1 * glm::vec3(resx / (float) mod_factor,
                            resy / (float) mod_factor,
                            resz / (float) mod_factor);
}

inline float dist2_max1(glm::vec3 a, glm::vec3 b)
{
    return fminf( glm::l2Norm(a - b), 1.f);
}

glm::vec3 acc_from_noise_smooth(glm::vec3 part_pos, int seed, float mag1, float size = 1.0f)
{
    int xi0 = part_pos.x - (fmodf(part_pos.x, size));
    int yi0 = part_pos.y - (fmodf(part_pos.y, size));
    int zi0 = part_pos.z - (fmodf(part_pos.z, size));
    int xi = part_pos.x + (size - fmodf(part_pos.x, size));
    int yi = part_pos.y + (size - fmodf(part_pos.y, size));
    int zi = part_pos.z + (size - fmodf(part_pos.z, size));

    glm::vec3 v0 = acc_from_noise(glm::vec3(xi0, yi0, zi0), seed, 1.0f);
    glm::vec3 v1 = acc_from_noise(glm::vec3(xi0, yi0, zi),  seed, 1.0f);
    glm::vec3 v2 = acc_from_noise(glm::vec3(xi0, yi,  zi0), seed, 1.0f);
    glm::vec3 v3 = acc_from_noise(glm::vec3(xi0, yi,  zi),  seed, 1.0f);
    glm::vec3 v4 = acc_from_noise(glm::vec3(xi,  yi0, zi0), seed, 1.0f);
    glm::vec3 v5 = acc_from_noise(glm::vec3(xi,  yi0, zi),  seed, 1.0f);
    glm::vec3 v6 = acc_from_noise(glm::vec3(xi,  yi,  zi0), seed, 1.0f);
    glm::vec3 v7 = acc_from_noise(glm::vec3(xi,  yi,  zi),  seed, 1.0f);

    glm::vec3 res = glm::vec3(0,0,0);

    res += (1.0f - dist2_max1(part_pos, v0)) * v0;
    res += (1.0f - dist2_max1(part_pos, v1)) * v1;
    res += (1.0f - dist2_max1(part_pos, v2)) * v2;
    res += (1.0f - dist2_max1(part_pos, v3)) * v3;
    res += (1.0f - dist2_max1(part_pos, v4)) * v4;
    res += (1.0f - dist2_max1(part_pos, v5)) * v5;
    res += (1.0f - dist2_max1(part_pos, v6)) * v6;
    res += (1.0f - dist2_max1(part_pos, v7)) * v7;

    return res * mag1;
}

ParticleComputer::ParticleComputer()
{}

std::shared_ptr<ParticleComputer> ParticleComputer::get() {
    if (!particle_computer_instance) {
        particle_computer_instance = std::shared_ptr<ParticleComputer>(new CPUParticleComputer);
    }
    return particle_computer_instance;
}

CPUParticleComputer::CPUParticleComputer()
{}

ParticleState CPUParticleComputer::interpolate(const ParticleState &particle_state,
                         const std::vector<Force> &forces,
                         TimeTick delta_tick,
                         const TimeSec &tick_per_sec,
                         EmitterGroup &emitters)
{
    ParticleState current_state = particle_state;

    while (delta_tick != 0)
    {
        if (delta_tick > 0) {
            current_state = step(particle_state, forces, 1 / (float)tick_per_sec, emitters);
            delta_tick--;
        } else {
            current_state = step(particle_state, forces, -1 / (float)tick_per_sec, emitters);
            delta_tick++;
        }
    }

    return current_state;
}

// TODO
// add lifetime mechanism for particles + rendering for shaders (intensity)
ParticleState CPUParticleComputer::step(const ParticleState &particle_state,
                                        const std::vector<Force> &forces,
                                        const TimeSec &delta_sec,
                                        EmitterGroup &emitters)
{
    float ds = (float) delta_sec;
    ParticleState new_state;
    int ps_size = particle_state.size();
    new_state.reserve(ps_size);

    //emit particles
    for (std::unique_ptr<Emitter>& emitter : emitters) {
        emitter->emitParticles([&new_state](Particle p){
            new_state.push_back(p);
        });
    }

    //calculate positions
    for (int i = 0; i < ps_size; i++)
    {
        Particle new_particle = particle_state[i];

        //update particle life
        new_particle.age += delta_sec;
        if (new_particle.age < 0 || new_particle.age > new_particle.life) {
            continue;
        }

        //calculate new velocity
        for (int j = 0; j < forces.size(); j++) {
            const Force &force = forces[j];
            switch (force.type) {
            case ForcePoint:
                new_particle.velocity += ds * acc_from_point_force(force.xyz(), new_particle.position, force.mag1);
                break;
            case ForceDirectional:
                new_particle.velocity += ds * acc_from_directional_force(force.xyz(), force.mag1);
                break;
            case ForceNoise_1:
                new_particle.velocity += ds * acc_from_noise(new_particle.position, force.seed1, force.mag1);
                break;
            case ForceNoise_2:
                new_particle.velocity += ds * acc_from_noise_smooth(new_particle.position, force.seed1, force.mag1, force.x);
                break;
            default:
                break;
            }
        }
        //calculate new position
        new_particle.position += new_particle.velocity;
        new_state.push_back(new_particle);
    }
    return new_state;
}
