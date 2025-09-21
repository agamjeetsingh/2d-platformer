//
// Created by Agamjeet Singh on 21/09/25.
//

#include "../../../include/physics/3d/Collision.h"
#include "../../../include/physics/3d/CollisionsHandler.h"
#include "../../../include/physics/3d/CollidableObject.h"
#include "../../../include/physics/3d/geometry/Box.h"

eng::d3::Collision::Collision(CollidableObject& objectA, CollidableObject& objectB, IncompleteCollision incomplete_collision,
                              std::size_t collidingBoxAIndex, std::size_t collidingBoxBIndex)
    : objectA(objectA), objectB(objectB), collidingBoxAIndex(collidingBoxAIndex), collidingBoxBIndex(collidingBoxBIndex),
      axis(incomplete_collision.collision_axis), deltaTime(incomplete_collision.deltaTime), collisionTime(incomplete_collision.collisionTime) {}

bool eng::d3::Collision::operator==(const Collision& other) const {
    return &objectA == &other.objectA &&
           &objectB == &other.objectB &&
           collidingBoxAIndex == other.collidingBoxAIndex &&
           collidingBoxBIndex == other.collidingBoxBIndex &&
           axis == other.axis &&
           deltaTime == other.deltaTime &&
           collisionTime == other.collisionTime;
}

eng::d3::Box eng::d3::Collision::getCollidingBoxA() const {
    return objectA.getHitbox().getBoxes()[collidingBoxAIndex];
}

eng::d3::Box eng::d3::Collision::getCollidingBoxB() const {
    return objectB.getHitbox().getBoxes()[collidingBoxBIndex];
}
