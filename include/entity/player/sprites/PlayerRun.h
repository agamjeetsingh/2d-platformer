//
// Created by Agamjeet Singh on 04/09/25.
//

#ifndef PLAYERRUN_H
#define PLAYERRUN_H
#include "PlayerTextures.h"
#include <SFML/Graphics.hpp>

class PlayerRun final : public PlayerTextures {
public:
    PlayerRun() {
        for (int i = 0; i < 12; i++) {
            sf::Texture texture;
            if (i <= 9) {
                assert(texture.loadFromFile("../assets/player/runFast/runFast0" + std::to_string(i) + ".png"));
            } else {
                assert(texture.loadFromFile("../assets/player/runFast/runFast" + std::to_string(i) + ".png"));
            }
            textures.push_back(texture);
        }

        for (int i = 0; i < 12; i++) {
            intervals.push_back(0.1);
        }
    }

    static const PlayerRun& getInstance() {
        static PlayerRun instance;
        return instance;
    }
};



#endif //PLAYERRUN_H
