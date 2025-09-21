//
// Created by Agamjeet Singh on 21/09/25.
//

#include "../../../include/physics/3d/PhysicsObject.h"

#include <iostream>
#include <sstream>

eng::d3::PhysicsObject::PhysicsObject(Vector3f position) : position(position) {}

eng::Vector3f eng::d3::PhysicsObject::getTotalVelocity() const {
    return base_velocity + friction_velocity;
}

void eng::d3::PhysicsObject::printVelocity(const std::string &name) const {
    std::cout << name << " base_velocity: x = " << base_velocity.x << ", y = " << base_velocity.y << std::endl;
    std::cout << name << " friction_velocity: x = " << friction_velocity.x << ", y = " << friction_velocity.y << std::endl;
}

void eng::d3::PhysicsObject::printVelocity() const {
    printVelocity(getAddressAsString());
}

std::string eng::d3::PhysicsObject::getAddressAsString() const {
    std::ostringstream oss;
    oss << this;
    return oss.str();
}
