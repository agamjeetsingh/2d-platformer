//
// Created by Agamjeet Singh on 27/07/25.
//

#include "events/Contact.h"

#include "entity/CollidableObject.h"

bool Contact::operator==(const Contact &other) const {
    return &objectA == &other.objectA &&
           &objectB == &other.objectB &&
           collidingRectAIndex == other.collidingRectAIndex &&
           collidingRectBIndex == other.collidingRectBIndex &&
           axis == other.axis;
}

sf::FloatRect Contact::getCollidingRectA() const {
    return objectA.getHitbox().getRects()[collidingRectAIndex];
}

sf::FloatRect Contact::getCollidingRectB() const {
    return objectB.getHitbox().getRects()[collidingRectBIndex];
}


