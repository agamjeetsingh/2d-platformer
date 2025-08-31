//
// Created by Agamjeet Singh on 09/08/25.
//

#ifndef DASHDIRECTION_H
#define DASHDIRECTION_H

#include <complex>
#include <SFML/Graphics.hpp>

#include "utility/SoundEffect.h"

enum class DashDirection {
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT
};

inline sf::Vector2f directionToVector(DashDirection direction) {
    switch (direction) {
        case DashDirection::UP: return sf::Vector2f{0, -1};
        case DashDirection::UP_RIGHT: return sf::Vector2f{1, -1}.normalized();
        case DashDirection::RIGHT: return sf::Vector2f{1, 0};
        case DashDirection::DOWN_RIGHT: return sf::Vector2f{1, 1}.normalized();
        case DashDirection::DOWN: return sf::Vector2f{0, 1};
        case DashDirection::DOWN_LEFT: return sf::Vector2f{-1, 1}.normalized();
        case DashDirection::LEFT: return sf::Vector2f{-1, 0};
        case DashDirection::UP_LEFT: return sf::Vector2f{-1, -1}.normalized();
        default: throw std::runtime_error("Invalid DashDirection");
    }
}

inline SoundEffect directionToSoundEffect(DashDirection direction) {
    switch (direction) {
        case DashDirection::UP:
        case DashDirection::UP_RIGHT:
        case DashDirection::RIGHT:
        case DashDirection::DOWN_RIGHT:
            return SoundEffect::DASH_RIGHT;

        case DashDirection::DOWN:
        case DashDirection::DOWN_LEFT:
        case DashDirection::LEFT:
        case DashDirection::UP_LEFT:
            return SoundEffect::DASH_LEFT;

        default:
            throw std::runtime_error("Invalid DashDirection");
    }
}

#endif //DASHDIRECTION_H
