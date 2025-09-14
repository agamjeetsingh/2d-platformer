//
// Created by Agamjeet Singh on 25/07/25.
//

#ifndef PLAYERSPRITEHANDLER_H
#define PLAYERSPRITEHANDLER_H

#include <numeric>
#include <unordered_map>

#include "../../sprites/TexturesHolder.h"
#include "PlayerSpriteState.h"
#include <SFML/Graphics/Sprite.hpp>

#include "Facing.h"

namespace sf {
    class Sprite;
}

class PlayerSpriteHandler {
public:
    explicit PlayerSpriteHandler(PlayerSpriteState& state, sf::Sprite& sprite, Facing& facing): facing(facing), sprite(sprite), state(state) {
        textures.emplace(PlayerSpriteState::GroundIdle, TexturesHolder(9, "../assets/player/idle/idle"));
        textures.emplace(PlayerSpriteState::Running, TexturesHolder(12, "../assets/player/runFast/runFast"));
        textures.emplace(PlayerSpriteState::Dashing, TexturesHolder(4, "../assets/player/dash/dash"));
        textures.emplace(PlayerSpriteState::Falling, TexturesHolder(8, "../assets/player/fall/fall"));
        textures.emplace(PlayerSpriteState::Dead, TexturesHolder(11, "../assets/player/death/death_h", std::vector(11, 0.03f)));
    }

    [[nodiscard]] float getAnimationLength(const PlayerSpriteState state) const {
        if (!textures.contains(state)) return 0;
        auto& intervals = textures.at(state).getIntervals();
        return std::accumulate(intervals.begin(), intervals.end(), 0.f);
    }

    void update(float deltaTime) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y});

        auto scale = (facing != prev_facing) ? sf::Vector2f{-1, 1} : sf::Vector2f{1, 1};
        prev_facing = facing;
        sprite.scale(scale);

        if (!textures.contains(state)) {
            return;
        }
        TexturesHolder& player_textures = textures.at(state);

        if (curr_state != state) {
            curr_state = state;
            player_textures.reset();
        }

        player_textures.update(deltaTime);

        sprite.setTexture(player_textures.getCurrentTexture());
        sprite.setPosition(sprite.getPosition() + sf::Vector2f{8, 12});
        if (facing == Facing::Left) {
            sprite.setPosition(sprite.getPosition() + sf::Vector2f{-3, 0});
        }
    }

private:
    Facing& facing;
    Facing prev_facing = facing;
    sf::Sprite& sprite;
    PlayerSpriteState& state;
    PlayerSpriteState curr_state = state;

    std::unordered_map<PlayerSpriteState, TexturesHolder> textures;
};



#endif //PLAYERSPRITEHANDLER_H
