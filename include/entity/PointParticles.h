//
// Created by Agamjeet Singh on 10/09/25.
//

#ifndef PARTICLES_H
#define PARTICLES_H

#include <random>
#include <SFML/Graphics.hpp>
#include <functional>
#include "utility/Random.h"
#include <ranges>
#include <algorithm>

struct Particle {
    Particle(sf::Vector2f velocity, float lifetime): velocity(velocity), lifetime(lifetime) {}
    sf::Vector2f velocity;
    const float lifetime;
    float time_left = lifetime;
};

class PointParticles {
public:
    struct ConstantColor {
        explicit ConstantColor(sf::Color color): color(color) {}
        sf::Color operator()(float dt, float lifetime) const {
            return color;
        }

    private:
        sf::Color color;
    };

    struct FadingColor {
        explicit FadingColor(sf::Color color): color(color) {}
        sf::Color operator()(float dt, float lifetime) {
            if (lifetime_left < 0) {
                lifetime_left = lifetime;
            }
            lifetime_left = std::max(0.f, lifetime_left - dt);
            alpha = (lifetime_left / lifetime) * 255.f;
            color.a = static_cast<uint8_t>(alpha);
            return color;
        }
    private:
        sf::Color color;
        float alpha = 255;
        float lifetime_left = -1;
    };

    struct ExpDampedVelocity {
        ExpDampedVelocity(sf::Vector2f initial_velocity, float damping_rate): initial_velocity(initial_velocity), damping_rate(damping_rate) {}

        sf::Vector2f operator()(float dt) {
            total_elapsed += dt;
            return initial_velocity * std::exp(-damping_rate * total_elapsed);
        }
    private:
        sf::Vector2f initial_velocity;
        float damping_rate;
        float total_elapsed = 0;
    };

    struct RandomSinWaveVelocity {
        RandomSinWaveVelocity(Random::FloatRange constant_horizontal_velocity_range, Random::FloatRange amplitude_range, Random::FloatRange wave_number_range, Random::FloatRange phase_range): constant_horizontal_velocity_range(constant_horizontal_velocity_range), amplitude_range(amplitude_range), wave_number_range(wave_number_range), phase_range(phase_range) {}

        sf::Vector2f operator()(float dt) {
            if (total_elapsed == 0) {
                constant_horizontal_velocity = constant_horizontal_velocity_range.getRandom();
                amplitude = amplitude_range.getRandom();
                wave_number = wave_number_range.getRandom();
                phase = phase_range.getRandom();
            }
            total_elapsed += dt;
            float vertical_velocity = amplitude * wave_number * constant_horizontal_velocity * std::cos(wave_number * constant_horizontal_velocity * total_elapsed + phase);
            return {constant_horizontal_velocity, vertical_velocity};
        }

    private:
        Random::FloatRange constant_horizontal_velocity_range;
        Random::FloatRange amplitude_range;
        Random::FloatRange wave_number_range;
        Random::FloatRange phase_range;

        float constant_horizontal_velocity = 0;
        float amplitude = 0;
        float wave_number = 0;
        float phase = 0;
        float total_elapsed = 0;
    };

    PointParticles(size_t num_particles, const std::function<sf::Color(float, float)>& get_color, const std::function<sf::Vector2f(float)>& get_velocity, const std::vector<sf::Vector2f> &positions, const std::vector<float>& lifetimes);

    PointParticles(size_t num_particles, const std::function<sf::Color(float, float)>& get_color, const std::function<sf::Vector2f(float)>& get_velocity, Random::Vector2fRange position_range, Random::FloatRange lifetime_range);

    // TODO - Remove dead particles, no point

    void updateSprite(float dt);

    std::optional<sf::VertexArray*> getSprite();

private:
    size_t num_particles;
    sf::VertexArray vertices{sf::PrimitiveType::Points, num_particles};
    std::vector<Particle> particles;
    std::vector<std::function<sf::Color(float, float)>> get_colors;
    std::vector<std::function<sf::Vector2f(float)>> get_velocities;
    std::vector<bool> dead_particles;
    float total_elapsed = 0;
    float max_lifetime;
};



#endif //PARTICLES_H
