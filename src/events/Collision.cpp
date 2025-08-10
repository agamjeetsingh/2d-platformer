//
// Created by Agamjeet Singh on 18/07/25.
//

#include "events/Collision.h"
#include "physics/CollisionsHandler.h"

Collision::Collision(CollidableObject& objectA, CollidableObject& objectB, IncompleteCollision incomplete_collision,
    std::size_t collidingRectAIndex,
    std::size_t collidingRectBIndex):
    objectA(objectA),
    objectB(objectB),
    collidingRectAIndex(collidingRectAIndex),
    collidingRectBIndex(collidingRectBIndex),
    axis(incomplete_collision.axis),
    deltaTime(incomplete_collision.deltaTime),
    collisionTime(incomplete_collision.collisionTime) {}

bool Collision::operator==(const Collision& other) const {
    return &objectA == &other.objectA &&
           &objectB == &other.objectB &&
           collidingRectAIndex == other.collidingRectAIndex &&
           collidingRectBIndex == other.collidingRectBIndex &&
           axis == other.axis &&
           deltaTime == other.deltaTime &&
           collisionTime == other.collisionTime;
}

sf::FloatRect Collision::getCollidingRectA() const {
    return objectA.getHitbox().getRects()[collidingRectAIndex];
}

sf::FloatRect Collision::getCollidingRectB() const {
    return objectB.getHitbox().getRects()[collidingRectBIndex];
}

