//
// Created by Agamjeet Singh on 18/07/25.
//

#ifndef COLLISION_H
#define COLLISION_H

#include <SFML/Graphics/Rect.hpp>
#include "../physics/CollisionAxis.h"

struct IncompleteCollision;
class CollidableObject;

struct Collision {
    Collision(CollidableObject& objectA, CollidableObject& objectB, IncompleteCollision incomplete_collision,
    std::size_t collidingRectAIndex,
    std::size_t collidingRectBIndex);

    CollidableObject& objectA;
    CollidableObject& objectB;
    std::size_t collidingRectAIndex;
    std::size_t collidingRectBIndex;
    CollisionAxis axis;
    float deltaTime;
    float collisionTime;

    bool operator==(const Collision& other) const;

    [[nodiscard]] sf::FloatRect getCollidingRectA() const;

    [[nodiscard]] sf::FloatRect getCollidingRectB() const;
};



#endif //COLLISION_H
