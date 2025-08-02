//
// Created by Agamjeet Singh on 25/07/25.
//

#ifndef PLAYERTEXTURES_H
#define PLAYERTEXTURES_H
#include <vector>


namespace sf {
    class Texture;
}

class PlayerTextures {
public:
    [[nodiscard]] const std::vector<float>& getIntervals() const {
        return intervals;
    }

    [[nodiscard]] const std::vector<sf::Texture>& getTextures() const {
        return textures;
    }

protected:
    std::vector<sf::Texture> textures;
    std::vector<float> intervals;
};



#endif //PLAYERTEXTURES_H
