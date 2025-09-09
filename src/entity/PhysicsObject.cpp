//
// Created by Agamjeet Singh on 13/07/25.
//

#include "entity/PhysicsObject.h"

PhysicsObject::PhysicsObject(sf::Sprite sprite, sf::Vector2f position = {0, 0}) :
    sprite(std::move(sprite)),
    position(position) {
    this->sprite.setPosition(position);
}

const sf::Vector2f &PhysicsObject::getPositionRef() const {
    return position;
}

const sf::Sprite* PhysicsObject::getSprite() const {
    return &sprite;
}

sf::Vector2f PhysicsObject::getPosition() const {
    return position;
}

void PhysicsObject::setPosition(sf::Vector2f position) {
    this->position = position;
    sprite.setPosition(this->position);
}

void PhysicsObject::addPosition(sf::Vector2f position) {
    this->position += position;
    sprite.setPosition(this->position);
}

sf::Vector2f PhysicsObject::getTotalVelocity() const {
    return base_velocity + friction_velocity;
}

void PhysicsObject::printVelocity(const std::string &name) const {
    std::cout << name << " base_velocity: x = " << base_velocity.x << ", y = " << base_velocity.y << std::endl;
    std::cout << name << " friction_velocity: x = " << friction_velocity.x << ", y = " << friction_velocity.y << std::endl;
}

void PhysicsObject::printVelocity() const {
    printVelocity(getAddressAsString());
}

std::string PhysicsObject::getAddressAsString() const {
    std::ostringstream oss;
    oss << this;
    return oss.str();
}

