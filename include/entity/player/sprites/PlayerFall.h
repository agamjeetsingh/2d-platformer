//
// Created by Agamjeet Singh on 04/09/25.
//

#ifndef PLAYERFALL_H
#define PLAYERFALL_H
#include "PlayerTextures.h"
#include <SFML/Graphics.hpp>


class PlayerFall final : public PlayerTextures {
public:
    PlayerFall() {
        for (int i = 0; i < 8; i++) {
            sf::Texture texture;
            assert(texture.loadFromFile("../assets/player/fall/fall0" + std::to_string(i) + ".png"));
            textures.push_back(texture);
        }

        for (int i = 0; i < 8; i++) {
            intervals.push_back(0.1);
        }
    }

    static const PlayerFall& getInstance() {
        static PlayerFall instance;
        return instance;
    }
};



#endif //PLAYERFALL_H
