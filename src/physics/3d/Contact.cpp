//
// Created by Agamjeet Singh on 22/09/25.
//

#include "../../../include/physics/3d/Contact.h"

#include "../../../include/physics/3d/CollidableObject.h"

eng::d3::Contact::Contact(Collision collision):
    objectA(collision.objectA),
    objectB(collision.objectB),
    collidingBoxAIndex(collision.collidingBoxAIndex),
    collidingBoxBIndex(collision.collidingBoxBIndex), axis(collision.axis) {}

eng::d3::Box eng::d3::Contact::getCollidingBoxA() const {
    return objectA.getHitbox().getBoxes()[collidingBoxAIndex];
}

eng::d3::Box eng::d3::Contact::getCollidingBoxB() const {
    return objectB.getHitbox().getBoxes()[collidingBoxBIndex];
}

bool eng::d3::Contact::operator==(const Contact &other) const {
    return &objectA == &other.objectA &&
           &objectB == &other.objectB &&
           collidingBoxAIndex == other.collidingBoxAIndex &&
           collidingBoxBIndex == other.collidingBoxBIndex &&
           axis == other.axis;
}


