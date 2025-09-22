//
// Created by Agamjeet Singh on 21/09/25.
//

#include "../../../include/physics/3d/CollidableObject.h"
#include "../../../include/physics/3d/CollisionsHandler.h"
#include "../../../include/physics/3d/Collision.h"

#include <cassert>

eng::d3::CollidableObject::CollidableObject(std::vector<Box> hitbox,
                                            Vector3f position,
                                            float mass) : PhysicsObject(position),
                                                          mass(mass),
                                                          hitbox({std::move(hitbox), this->position}){
    assert(mass >= 0);
}

[[nodiscard]] float eng::d3::CollidableObject::getInvMass() const {
    return (mass == 0) ? 0 : (1.0f / mass);
}

[[nodiscard]] const eng::d3::Hitbox& eng::d3::CollidableObject::getHitbox() const {
    return hitbox;
}

bool eng::d3::CollidableObject::operator==(const CollidableObject &other) const {
    return this == &other;
}

bool eng::d3::CollidableObject::canCollideWith(const CollidableObject& other, Collision collision) const {
    return true;
}