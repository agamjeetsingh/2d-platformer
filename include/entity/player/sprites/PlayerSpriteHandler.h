//
// Created by Agamjeet Singh on 25/07/25.
//

#ifndef PLAYERSPRITEHANDLER_H
#define PLAYERSPRITEHANDLER_H

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
    }

    void update(float deltaTime) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y});

        auto scale = (facing == Facing::Left) ? sf::Vector2f{-1, 1} : sf::Vector2f{1, 1};
        sprite.setScale(scale);

        if (!textures.contains(state)) {
            return;
        }
        const TexturesHolder& player_textures = textures.at(state);

        if (curr_state != state) {
            curr_state = state;
            time_in_state = 0;
            curr_sprite_index = 0;
        }

        time_in_state += deltaTime;

        while (player_textures.getIntervals()[curr_sprite_index] <= time_in_state) {
            if (curr_sprite_index == player_textures.getTextures().size()) {
                curr_sprite_index = 0;
            }

            time_in_state -= player_textures.getIntervals()[curr_sprite_index++];
        }

        sprite.setTexture(player_textures.getTextures()[curr_sprite_index]);
        sprite.setPosition(sprite.getPosition() + sf::Vector2f{8, 12});
        if (facing == Facing::Left) {
            sprite.setPosition(sprite.getPosition() + sf::Vector2f{-3, 0});
        }
    }

private:
    Facing& facing;
    sf::Sprite& sprite;
    PlayerSpriteState& state;
    PlayerSpriteState curr_state = state;
    float time_in_state = 0;
    size_t curr_sprite_index = 0;

    std::unordered_map<PlayerSpriteState, TexturesHolder> textures;
};



#endif //PLAYERSPRITEHANDLER_H
