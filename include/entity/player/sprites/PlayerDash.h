//
// Created by Agamjeet Singh on 04/09/25.
//

#ifndef PLAYERDASH_H
#define PLAYERDASH_H
#include "PlayerTextures.h"
#include <SFML/Graphics.hpp>

class PlayerDash final : public PlayerTextures {
public:
    PlayerDash() {
        for (int i = 0; i < 4; i++) {
            sf::Texture texture;
            assert(texture.loadFromFile("../assets/player/dash/dash0" + std::to_string(i) + ".png"));
            textures.push_back(texture);
        }

        for (int i = 0; i < 4; i++) {
            intervals.push_back(0.1);
        }
    }

    static const PlayerDash& getInstance() {
        static PlayerDash instance;
        return instance;
    }
};



#endif //PLAYERDASH_H