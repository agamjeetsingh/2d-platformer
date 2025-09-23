//
// Created by Agamjeet Singh on 22/09/25.
//

#ifndef CONTACT_H
#define CONTACT_H
#include <algorithm>

#include "Collision.h"
#include "geometry/Box.h"


namespace eng::d3 {
    struct Contact {
        explicit Contact(Collision collision);

        CollidableObject& objectA;
        CollidableObject& objectB;
        std::size_t collidingBoxAIndex;
        std::size_t collidingBoxBIndex;
        Vector3f axis;

        [[nodiscard]] Box getCollidingBoxA() const;

        [[nodiscard]] Box getCollidingBoxB() const;

        bool operator==(const Contact& other) const;
    };
}



#endif //CONTACT_H
