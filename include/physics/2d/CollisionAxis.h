//
// Created by Agamjeet Singh on 19/07/25.
//

#ifndef COLLISIONAXIS_H
#define COLLISIONAXIS_H

#include <SFML/Graphics.hpp>

enum class CollisionAxis {
    Up,
    Down,
    Left,
    Right
};

inline sf::Vector2f axisToVector(CollisionAxis axis) {
    switch (axis) {
        case CollisionAxis::Up: return {0, -1};
        case CollisionAxis::Down: return {0, 1};
        case CollisionAxis::Left: return {-1, 0};
        case CollisionAxis::Right: return {1, 0};
        default: return {0, 0};
    }
}


#endif //COLLISIONAXIS_H
