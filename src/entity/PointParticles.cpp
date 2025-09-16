//
// Created by Agamjeet Singh on 11/09/25.
//

#include "entity/PointParticles.h"

PointParticles::PointParticles(size_t num_particles, const std::function<sf::Color(float, float)> &get_color, const std::function<sf::Vector2f(float)> &get_velocity, const std::vector<sf::Vector2f> &positions, const std::vector<float> &lifetimes): num_particles(num_particles), dead_particles(std::vector<bool>(num_particles, false)), max_lifetime(lifetimes.empty() ? 0 : *std::ranges::max_element(lifetimes)) {
    for (int i = 0; i < num_particles; i++) {
        get_colors.push_back(get_color);
        get_velocities.push_back(get_velocity);
        vertices[i].position = positions[i];
        vertices[i].color = get_colors[i](0, lifetimes[i]);
        particles.emplace_back(get_velocities[i](0), lifetimes[i]);
    }
}

PointParticles::PointParticles(size_t num_particles, const std::function<sf::Color(float, float)> &get_color, const std::function<sf::Vector2f(float)> &get_velocity, Random::Vector2fRange position_range, Random::FloatRange lifetime_range): num_particles(num_particles), dead_particles(std::vector<bool>(num_particles, false)), max_lifetime(0) {
    for (int i = 0; i < num_particles; i++) {
        get_colors.push_back(get_color);
        get_velocities.push_back(get_velocity);
        vertices[i].position = position_range.getRandom();
        float lifetime = lifetime_range.getRandom();
        vertices[i].color = get_colors[i](0, lifetime);
        particles.emplace_back(get_velocities[i](0), lifetime);
        max_lifetime = std::max(max_lifetime, lifetime);
    }
}

void PointParticles::updateSprite(float dt) {
    total_elapsed += dt;
    for (int i = 0; i < num_particles; i++) {
        particles[i].time_left -= dt;
        if (particles[i].time_left <= 0) {
            dead_particles[i] = true;
        } else {
            vertices[i].color = get_colors[i](dt, particles[i].lifetime);
            particles[i].velocity = get_velocities[i](dt);
            vertices[i].position += particles[i].velocity * dt;
        }
    }
}

std::optional<sf::VertexArray *> PointParticles::getSprite() {
    if (total_elapsed >= max_lifetime) {
        return std::nullopt;
    }
    return &vertices;
}



