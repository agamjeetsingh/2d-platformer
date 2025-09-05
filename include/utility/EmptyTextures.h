//
// Created by Agamjeet Singh on 05/09/25.
//

#ifndef EMPTYTEXTURES_H
#define EMPTYTEXTURES_H
#include <vector>

#include <SFML/Graphics/Texture.hpp>

struct HashVector2u {
    std::size_t operator()(const sf::Vector2u& vector) const noexcept {
        return (static_cast<std::size_t>(vector.x) << 32) ^ static_cast<std::size_t>(vector.y);
    }
};

class EmptyTextures {
public:
    sf::Texture& getEmpty(sf::Vector2u size) {
        if (emptyTextures.contains(size)) {
            return emptyTextures[size];
        }
        sf::Texture empty_texture;
        assert(empty_texture.resize(size));
        emptyTextures.emplace(size, empty_texture);
        return emptyTextures[size];
    }

    static EmptyTextures& getInstance() {
        static EmptyTextures instance;
        return instance;
    }

    void cleanup() {
        emptyTextures.clear();
    }

private:
    std::unordered_map<sf::Vector2u, sf::Texture, HashVector2u> emptyTextures;
};



#endif //EMPTYTEXTURES_H
