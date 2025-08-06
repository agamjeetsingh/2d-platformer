//
// Created by Agamjeet Singh on 19/07/25.
//

#ifndef COLLISIONRESOLUTION_H
#define COLLISIONRESOLUTION_H

#include "../events/Collision.h"

class CollisionResolution {
public:

    static void resolve(Collision collision) {
        if (collision.objectA.type == CollidableObjectType::Movable && collision.objectB.type == CollidableObjectType::Immovable) {
            if (collision.axis == CollisionAxis::Down || collision.axis == CollisionAxis::Up) {
                collision.objectA.intrinsic_velocity.y = 0;
            } else {
                collision.objectA.intrinsic_velocity.x = 0;
            }
        }
        else if (collision.objectA.type == CollidableObjectType::Immovable && collision.objectB.type == CollidableObjectType::Movable) {
            if (collision.axis == CollisionAxis::Down || collision.axis == CollisionAxis::Up) {
                collision.objectB.intrinsic_velocity.y = 0;
            } else {
                collision.objectB.intrinsic_velocity.x = 0;
            }
        }
    }
};



#endif //COLLISIONRESOLUTION_H
