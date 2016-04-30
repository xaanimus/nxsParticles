#include "texturegenerator.h"

#include <cmath>
#include <iostream>

#define PARTICLE_SIZE_ONE 32

static std::vector<uint8_t> particle_tex_1;

std::vector<uint8_t>& tex_gen::particle_texture_1()
{
    if (particle_tex_1.size() < 1) {
        particle_tex_1.reserve(PARTICLE_SIZE_ONE * PARTICLE_SIZE_ONE * 3);

        const unsigned mid_size = PARTICLE_SIZE_ONE / 2;
        const uint8_t intensity = 255;
        const uint8_t r_mul_factor = intensity / mid_size;

        for (int i = 0; i < PARTICLE_SIZE_ONE; i++) {
            for (int j = 0; j < PARTICLE_SIZE_ONE; j++) {
                uint8_t r = sqrtf( powf(i-(int)mid_size, 2.f) + powf(j-(int)mid_size, 2.f) );
                uint8_t val = intensity - fmin(r * 1.5 * r_mul_factor, intensity);
                particle_tex_1[i * PARTICLE_SIZE_ONE * 3 + j * 3] = val;
                particle_tex_1[i * PARTICLE_SIZE_ONE * 3 + j * 3 + 1] = val;
                particle_tex_1[i * PARTICLE_SIZE_ONE * 3 + j * 3 + 2] = val;
            }
        }

    }
    return particle_tex_1;
}

int tex_gen::particle_tex_1_size() {
    return PARTICLE_SIZE_ONE;
}
