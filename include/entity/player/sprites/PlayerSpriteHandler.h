//
// Created by Agamjeet Singh on 25/07/25.
//

#ifndef PLAYERSPRITEHANDLER_H
#define PLAYERSPRITEHANDLER_H

#include <unordered_map>

#include "PlayerIdle.h"
#include "PlayerTextures.h"
#include "../PlayerSpriteState.h"
#include <SFML/Graphics/Sprite.hpp>

#include "PlayerDash.h"
#include "PlayerFall.h"
#include "PlayerRun.h"
#include "../Facing.h"

namespace sf {
    class Sprite;
}

class PlayerSpriteHandler {
public:
    explicit PlayerSpriteHandler(PlayerSpriteState& state, sf::Sprite& sprite, Facing& facing): state(state), sprite(sprite), facing(facing) {
        textures[PlayerSpriteState::GroundIdle] = PlayerIdle::getInstance();
        textures[PlayerSpriteState::Running] = PlayerRun::getInstance();
        textures[PlayerSpriteState::Dashing] = PlayerDash::getInstance();
        textures[PlayerSpriteState::Falling] = PlayerFall::getInstance();
        // Add all types
    }

    PlayerSpriteState& state;
    sf::Sprite& sprite;
    Facing& facing;

    void update(float deltaTime) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y});

        auto scale = (facing == Facing::Left) ? sf::Vector2f{-1, 1} : sf::Vector2f{1, 1};
        sprite.setScale(scale);

        const PlayerTextures& player_textures = textures[state];

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
    PlayerSpriteState curr_state = state;
    float time_in_state = 0;
    size_t curr_sprite_index = 0;

    std::unordered_map<PlayerSpriteState, PlayerTextures> textures;
};



#endif //PLAYERSPRITEHANDLER_H
