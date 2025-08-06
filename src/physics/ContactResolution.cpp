//
// Created by Agamjeet Singh on 24/07/25.
//

#include "../../include/physics/ContactResolution.h"

void ContactResolution::resolve(Collision collision) {
    // TODO - improve later if needed

    CollidableObject& object = (collision.objectA.type == CollidableObjectType::Movable) ? collision.objectA : collision.objectB;

    if (collision.axis == CollisionAxis::Down || collision.axis == CollisionAxis::Up) {
        object.intrinsic_velocity.y = 0;
    } else {

        object.intrinsic_velocity.x = 0;
    }
}