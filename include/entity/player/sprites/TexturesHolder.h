//
// Created by Agamjeet Singh on 25/07/25.
//

#ifndef PLAYERTEXTURES_H
#define PLAYERTEXTURES_H
#include <vector>
#include <SFML/Graphics.hpp>

class TexturesHolder {
public:
    TexturesHolder(int count, const std::string& basePath, const std::vector<float>& customIntervals = {}) {
        textures.reserve(count);
        intervals.reserve(count);

        for (int i = 0; i < count; i++) {
            sf::Texture texture;
            std::string filename = basePath + (i < 10 ? "0" : "") + std::to_string(i) + ".png";

            assert(texture.loadFromFile(filename));
            textures.push_back(std::move(texture));
        }

        if (!customIntervals.empty()) {
            assert(customIntervals.size() == static_cast<size_t>(count));
            intervals = customIntervals;
        } else {
            intervals.assign(count, 0.1f);
        }
    }

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
