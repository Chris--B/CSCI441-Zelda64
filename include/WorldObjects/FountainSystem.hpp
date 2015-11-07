#pragma once
#include "Utils.hpp"

#include "ParticleSystem.hpp"

namespace Internal {
struct FountainParticle {
    Vec pos;               // meters
    Vec vel;               // meters / second
    float mass     = 1.0;  // g
    float lifetime = 10.0; // seconds
};
};

class FountainSystem : public ParticleSystem<Internal::FountainParticle> {
public:
    using Particle = Internal::FountainParticle;
    // Note that this is negative.
    static const double s_gravity; // meters / second^2
    static const double s_dampen;  // ???

    virtual void update(double t, double dt) override;
    virtual void drawParticle(const Particle &particle) const override;

private:
};
