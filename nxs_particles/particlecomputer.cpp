#include "particlecomputer.h"
#include "glm/gtx/norm.hpp"

std::shared_ptr<ParticleComputer> particle_computer_instance;

/** calculates the acceleration vector on particle for a point force */
glm::vec3 acc_from_point_force(glm::vec3 force_pos, glm::vec3 part_pos, float mag1)
{
    glm::vec3 r = force_pos - part_pos;
    float r_mag = glm::l2Norm(r);
    return mag1 * r / (r_mag * r_mag * r_mag);
}

/** calculates the acceleration vector on particle for a directional force */
glm::vec3 acc_from_directional_force(glm::vec3 direction_unit, float mag1)
{
    return direction_unit * mag1;
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
                         const TimeSec &tick_per_sec)
{
    ParticleState current_state = particle_state;

    while (delta_tick != 0)
    {
        if (delta_tick > 0) {
            current_state = step(particle_state, forces, -1 / (float)tick_per_sec);
            delta_tick--;
        } else {
            current_state = step(particle_state, forces, 1 / (float)tick_per_sec);
            delta_tick++;
        }
    }

    return current_state;
}

ParticleState CPUParticleComputer::step(const ParticleState &particle_state,
                                        const std::vector<Force> &forces,
                                        const TimeSec &delta_sec)
{
    float ds = (float) delta_sec;
    ParticleState new_state;
    int ps_size = particle_state.size();
    new_state.reserve(ps_size);
    for (int i = 0; i < ps_size; i++)
    {
        Particle new_particle = particle_state[i];
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
