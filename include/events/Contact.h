//
// Created by Agamjeet Singh on 27/07/25.
//

#ifndef CONTACT_H
#define CONTACT_H

#include "../physics/CollisionAxis.h"
#include "../physics/CollisionsHandler.h"
#include "Collision.h"
#include "../physics/ContactAxis.h"

struct Contact {
    explicit Contact(Collision collision) :
    objectA(collision.objectA),
    objectB(collision.objectB),
    collidingRectA(collision.collidingRectA),
    collidingRectB(collision.collidingRectB) {
        axis = (collision.axis == CollisionAxis::Down || collision.axis == CollisionAxis::Up) ? ContactAxis::Y : ContactAxis::X;
    }


    CollidableObject& objectA;
    CollidableObject& objectB;
    sf::FloatRect collidingRectA;
    sf::FloatRect collidingRectB;
    ContactAxis axis;

    bool operator==(const Contact& other) const;

    [[nodiscard]] float contactLength() const {
        if (axis == ContactAxis::Y) {
            const float left = std::max(collidingRectA.position.x, collidingRectB.position.x);
            const float right = std::min(collidingRectA.position.x + collidingRectA.size.x, collidingRectB.position.x + collidingRectB.size.x);
            return (right - left > 0) ? right - left : 0;
        } else {
            const float top = std::max(collidingRectA.position.y, collidingRectB.position.y);
            const float bottom = std::min(collidingRectA.position.y + collidingRectA.size.y, collidingRectB.position.y + collidingRectB.size.y);
            return (bottom - top > 0) ? bottom - top : 0;
        }
    }
};



#endif //CONTACT_H
