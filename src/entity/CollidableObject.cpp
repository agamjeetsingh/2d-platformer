//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../include/entity/CollidableObject.h"
#include <iostream>
#include <utility>

#include "../../include/events/Collision.h"

CollidableObject::CollidableObject(std::vector<sf::FloatRect> hitbox,
                                   sf::Sprite sprite,
                                   sf::Vector2f position,
                                   CollidableObjectType type) : PhysicsObject(std::move(sprite), position),
                                                                type(type),
                                                                hitbox({std::move(hitbox), this->getPositionRef()}){
    CollisionsHandler::getInstance().addObject(*this);
}

[[nodiscard]] const Hitbox& CollidableObject::getHitbox() const {
    return hitbox;
}

bool CollidableObject::operator==(const CollidableObject &other) const {
    return this == &other;
}
