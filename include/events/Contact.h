//
// Created by Agamjeet Singh on 27/07/25.
//

#ifndef CONTACT_H
#define CONTACT_H

#include "../physics/CollisionAxis.h"
#include "Collision.h"
#include "../physics/ContactAxis.h"

struct Contact {
    explicit Contact(Collision collision) :
    objectA(collision.objectA),
    objectB(collision.objectB),
    collidingRectAIndex(collision.collidingRectAIndex),
    collidingRectBIndex(collision.collidingRectBIndex) {
        axis = (collision.axis == CollisionAxis::Down || collision.axis == CollisionAxis::Up) ? ContactAxis::Y : ContactAxis::X;
    }


    CollidableObject& objectA;
    CollidableObject& objectB;
    std::size_t collidingRectAIndex;
    std::size_t collidingRectBIndex;
    ContactAxis axis;

    [[nodiscard]] sf::FloatRect getCollidingRectA() const;

    [[nodiscard]] sf::FloatRect getCollidingRectB() const;

    bool operator==(const Contact& other) const;

    [[nodiscard]] float contactLength() const {
        if (axis == ContactAxis::Y) {
            const float left = std::max(getCollidingRectA().position.x, getCollidingRectB().position.x);
            const float right = std::min(getCollidingRectA().position.x + getCollidingRectA().size.x, getCollidingRectB().position.x + getCollidingRectB().size.x);
            return (right - left > 0) ? right - left : 0;
        } else {
            const float top = std::max(getCollidingRectA().position.y, getCollidingRectB().position.y);
            const float bottom = std::min(getCollidingRectA().position.y + getCollidingRectA().size.y, getCollidingRectB().position.y + getCollidingRectB().size.y);
            return (bottom - top > 0) ? bottom - top : 0;
        }
    }
};



#endif //CONTACT_H
