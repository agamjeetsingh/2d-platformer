//
// Created by Agamjeet Singh on 18/07/25.
//

#include "../../include/events/Collision.h"

Collision::Collision(CollidableObject& objectA, CollidableObject& objectB, IncompleteCollision incomplete_collision):
    objectA(objectA),
    objectB(objectB),
    collidingRectA(incomplete_collision.collidingRectA),
    collidingRectB(incomplete_collision.collidingRectB),
    axis(incomplete_collision.axis),
    deltaTime(incomplete_collision.deltaTime),
    collisionTime(incomplete_collision.collisionTime) {}

bool Collision::operator==(const Collision& other) const {
    return &objectA == &other.objectA &&
           &objectB == &other.objectB &&
           collidingRectA == other.collidingRectA &&
           collidingRectB == other.collidingRectB &&
           axis == other.axis &&
           deltaTime == other.deltaTime &&
           collisionTime == other.collisionTime;
}