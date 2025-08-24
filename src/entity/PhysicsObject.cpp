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

void PhysicsObject::setGravityVelocity(sf::Vector2f velocity) {
    assert(velocity.x == 0);
    gravity_velocity = velocity;
    gravity_velocity.y = std::min(gravity_velocity.y, MAX_FALL);
}

void PhysicsObject::addGravityVelocity(sf::Vector2f velocity) {
    assert(velocity.x == 0);
    gravity_velocity += velocity;
    gravity_velocity.y = std::min(gravity_velocity.y, MAX_FALL);
}

const sf::Sprite& PhysicsObject::getSprite() const {
    return sprite;
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
    return intrinsic_velocity + friction_velocity + impulse_velocity + gravity_velocity;
}

void PhysicsObject::printVelocity(const std::string &name) const {
    std::cout << name << " intrinsic_velocity: x = " << intrinsic_velocity.x << ", y = " << intrinsic_velocity.y << std::endl;
    std::cout << name << " friction_velocity: x = " << friction_velocity.x << ", y = " << friction_velocity.y << std::endl;
    std::cout << name << " impulse_velocity: x = " << impulse_velocity.x << ", y = " << impulse_velocity.y << std::endl;
}

void PhysicsObject::printVelocity() const {
    printVelocity(getAddressAsString());
}

std::string PhysicsObject::getAddressAsString() const {
    std::ostringstream oss;
    oss << this;
    return oss.str();
}

