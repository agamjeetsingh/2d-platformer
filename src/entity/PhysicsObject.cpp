//
// Created by Agamjeet Singh on 13/07/25.
//

#include "entity/PhysicsObject.h"

PhysicsObject::PhysicsObject(sf::Sprite sprite, sf::Vector2f position = {0, 0}) :
    sprite(std::move(sprite)),
    position(position) {
    this->sprite.setPosition(position);
}

void PhysicsObject::update(float deltaTime) {
    sprite.setPosition(position);
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
    return intrinsic_velocity + friction_velocity + impulse_velocity;
}


