//
// Created by Agamjeet Singh on 13/07/25.
//

#include "entity/CollidableObject.h"
#include <iostream>
#include <utility>

#include "events/Collision.h"
#include "entity/player/Player.h"
#include "physics/CollisionsHandler.h"

CollidableObject::CollidableObject(std::vector<sf::FloatRect> hitbox,
                                   sf::Sprite sprite,
                                   sf::Vector2f position,
                                   CollidableObjectType type,
                                   float mass) : PhysicsObject(std::move(sprite), position),
                                                                type(type),
                                                                mass(type == CollidableObjectType::Immovable ? 0 : mass),
                                                                hitbox({std::move(hitbox), this->getPositionRef()}){
    assert(mass >= 0);
    if (type == CollidableObjectType::Movable) {
        assert(mass != 0);
    }
    CollisionsHandler::getInstance().addObject(*this);
}

[[nodiscard]] float CollidableObject::getInvMass() const {
    return (mass == 0) ? 0 : (1.0f / mass);
}


[[nodiscard]] const Hitbox& CollidableObject::getHitbox() const {
    return hitbox;
}

bool CollidableObject::operator==(const CollidableObject &other) const {
    return this == &other;
}

[[nodiscard]] Player* CollidableObject::isPlayer() {
    return dynamic_cast<Player*>(this);
}

[[nodiscard]] const Player *CollidableObject::isPlayer() const {
    return dynamic_cast<const Player*>(this);
}

bool CollidableObject::canCollideWith(const CollidableObject& other, Collision collision) const {
    return true;
}
