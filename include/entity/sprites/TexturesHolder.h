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

    void update(const float dt) {
        time_in_state += dt;

        while (intervals[texture_index] <= time_in_state) {
            if (texture_index == textures.size()) {
                texture_index = 0;
            }
            time_in_state -= intervals[texture_index++];
        }
    }

    [[nodiscard]] const sf::Texture& getCurrentTexture() const {
        return textures[texture_index];
    }

    void reset() {
        texture_index = 0;
        time_in_state = 0;
    }

protected:
    std::vector<sf::Texture> textures;
    std::vector<float> intervals;
    size_t texture_index = 0;
    float time_in_state = 0;
};



#endif //PLAYERTEXTURES_H
