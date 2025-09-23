//
// Created by Agamjeet Singh on 21/09/25.
//

#ifndef COLLISION3D_H
#define COLLISION3D_H
#include <cstdio>

#include "geometry/Vector3f.h"

namespace eng::d3 {
    class CollidableObject;
    struct IncompleteCollision;
    class Box;
    
    struct Collision {
        Collision(CollidableObject& objectA, CollidableObject& objectB, IncompleteCollision incomplete_collision,
        std::size_t collidingBoxAIndex,
        std::size_t collidingBoxBIndex);

        CollidableObject& objectA;
        CollidableObject& objectB;
        std::size_t collidingBoxAIndex;
        std::size_t collidingBoxBIndex;
        Vector3f axis;
        float deltaTime;
        float collisionTime;

        bool operator==(const Collision& other) const;

        [[nodiscard]] Box getCollidingBoxA() const;

        [[nodiscard]] Box getCollidingBoxB() const;
    };
}



#endif //COLLISION3D_H
