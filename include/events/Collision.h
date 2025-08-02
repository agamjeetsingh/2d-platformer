//
// Created by Agamjeet Singh on 18/07/25.
//

#ifndef COLLISION_H
#define COLLISION_H

#include "../../../../../../opt/homebrew/Cellar/sfml/3.0.1/include/SFML/Graphics/Rect.hpp"
#include "../physics/CollisionAxis.h"
#include "../physics/CollisionsHandler.h"

struct IncompleteCollision;
class CollidableObject;

struct Collision {
    Collision(CollidableObject& objectA, CollidableObject& objectB, IncompleteCollision incomplete_collision);

    CollidableObject& objectA;
    CollidableObject& objectB;
    sf::FloatRect collidingRectA;
    sf::FloatRect collidingRectB;
    CollisionAxis axis;
    float deltaTime;
    float collisionTime;

    bool operator==(const Collision& other) const;
};



#endif //COLLISION_H
