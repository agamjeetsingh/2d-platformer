//
// Created by Agamjeet Singh on 11/09/25.
//

#ifndef SPRITEPARTICLES_H
#define SPRITEPARTICLES_H
#include <unordered_set>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ranges>
#include <functional>

#include "sprites/TexturesHolder.h"
#include "utility/GameRender.h"

class SpriteParticles {
public:
    struct ConstantVelocity {
        explicit ConstantVelocity(sf::Vector2f velocity): velocity(velocity) {}
        sf::Vector2f operator()(float dt) const {
            return velocity;
        }

    private:
        sf::Vector2f velocity;
    };

    explicit SpriteParticles(const std::vector<TexturesHolder>& texture_holders, const std::vector<sf::Vector2f>& positions, const std::vector<float>& lifeTimes, const std::function<sf::Vector2f(float)>& get_velocity): positions(positions), texture_holders(texture_holders), lifeTimes(lifeTimes), active_particles(std::vector(texture_holders.size(), true)) {
        assert(texture_holders.size() == positions.size());
        assert(positions.size() == lifeTimes.size());
        for (int i = 0; i < texture_holders.size(); i++) {
            get_velocities.push_back(get_velocity);
        }
    }

    void updateSprite(float dt) {
        for (int i = 0; i < texture_holders.size(); i++) {
            lifeTimes[i] -= dt;
            if (lifeTimes[i] <= 0) {
                active_particles[i] = false;
            } else {
                positions[i] += get_velocities[i](dt) * dt;
                texture_holders[i].update(dt);
            }
        }
    }

    std::vector<sf::Drawable*> getSprite() {
        // TODO - Make it so I am not reconstructing every frame
        sprites.clear();
        for (int i = 0; i < texture_holders.size(); i++) {
            if (active_particles[i]) {
                sprites.emplace_back(texture_holders[i].getCurrentTexture());
                sprites.back().setPosition(positions[i]);
                sprites.back().setColor(sf::Color::Black); // TODO - Temporary
            }
        }
        auto view = sprites | std::ranges::views::transform([](sf::Sprite& x) { return &x; });
        return {view.begin(), view.end()};
    }

private:
    std::vector<sf::Vector2f> positions;
    std::vector<TexturesHolder> texture_holders;
    std::vector<sf::Sprite> sprites;
    std::vector<float> lifeTimes;
    std::vector<bool> active_particles;
    std::vector<std::function<sf::Vector2f(float)>> get_velocities;
};

class SpriteParticlesManager {
public:
    static SpriteParticlesManager& getInstance() {
        static SpriteParticlesManager instance;
        return instance;
    }

    void createParticles(const std::vector<TexturesHolder>& texture_holders, const std::vector<sf::Vector2f>& positions, const std::vector<float>& lifeTimes, const std::function<sf::Vector2f(float)>& get_velocity) {
        removeExpiredParticles();
        auto shared_ptr = std::make_shared<SpriteParticles>(texture_holders, positions, lifeTimes, get_velocity);
        sprite_particles.insert(shared_ptr);
        GameRender::getInstance().registerDrawable(shared_ptr);
    }
private:
    std::unordered_set<std::shared_ptr<SpriteParticles>> sprite_particles;

    void removeExpiredParticles() {
        auto it = sprite_particles.begin();
        while (it != sprite_particles.end()) {
            if (it->get()->getSprite().empty()) {
                it = sprite_particles.erase(it);
            } else {
                ++it;
            }
        }
    }
};



#endif //SPRITEPARTICLES_H
