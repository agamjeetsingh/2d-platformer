//
// Created by Agamjeet Singh on 25/07/25.
//

#ifndef PLAYERIDLE_H
#define PLAYERIDLE_H
#include "PlayerTextures.h"
#include <SFML/Graphics.hpp>


class PlayerIdle final : public PlayerTextures {
public:
    PlayerIdle() {
        for (int i = 0; i < 9; i++) {
            sf::Texture texture;
            assert(texture.loadFromFile("../assets/player/idle0" + std::to_string(i) + ".png"));
            textures.push_back(texture);
        }

        for (int i = 0; i < 9; i++) {
            intervals.push_back(0.1);
        }
    }

    static const PlayerIdle& getInstance() {
        static PlayerIdle instance;
        return instance;
    }
};



#endif //PLAYERIDLE_H
