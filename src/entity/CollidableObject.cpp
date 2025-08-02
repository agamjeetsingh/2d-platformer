//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../include/entity/CollidableObject.h"
#include <iostream>
#include <utility>

#include "../../include/events/Collision.h"
#include "../../include/physics/GlobalIsland.h"
#include "../../include/physics/Island.h"

CollidableObject::CollidableObject(std::vector<sf::FloatRect> hitbox,
                                   sf::Sprite sprite,
                                   sf::Vector2f position,
                                   CollidableObjectType type,
                                   std::shared_ptr<Island> island) : PhysicsObject(std::move(sprite), position),
                                                                type(type),
                                                                hitbox({std::move(hitbox), this->getPositionRef()}),
                                                                island(std::move(island)) {
    CollisionsHandler::getInstance().addObject(*this);
    if (this->island != nullptr && this->island == GlobalIsland::getInstance()) { // If default
        std::cout << "Before: box island == global? " << (this->island == GlobalIsland::getInstance()) << std::endl;
        this->island = (type == CollidableObjectType::Movable) ? GlobalIsland::getInstance() : std::make_shared<Island>(*this);
        std::cout << "After: box island == global? " << (this->island == GlobalIsland::getInstance()) << std::endl;
    }
    if (this->island != nullptr && type != CollidableObjectType::Immovable) {
        joinIsland(this->island);
    }
}

// void CollidableObject::update(float deltaTime) {
//     const auto collision = collider.addPosition(velocity * deltaTime);
//     if (collision) {
//         std::cout << "Collision!!! x: " << getPosition().x << " y: " << getPosition().y << " other x: " << collision->getPosition().x << " other y: " << collision->getPosition().y <<std::endl;
//         // Handle collision
//         assert(collider != collision);
//         return; // Temporary
//     }
//     // Not sure whether I should update velocity first or acceleration, or whether it even matters
//     assert(!collision);
//     std::cout << "No collision x: " << getPosition().x << " y: " << getPosition().y << std::endl;
//     // Assuming no collision for now (after the if statement)
//
//     addPosition(velocity * deltaTime);
//
//     velocity += acceleration * deltaTime;
//     assert(collider.getPosition() == getPosition());
// }

[[nodiscard]] const Hitbox& CollidableObject::getHitbox() const {
    return hitbox;
}

bool CollidableObject::operator==(const CollidableObject &other) const {
    return this == &other;
}

sf::Vector2f CollidableObject::getIslandVelocity() const {
    return island->getLeaderVelocity();
}

sf::Vector2f CollidableObject::getIslandAcceleration() const {
    return island->getLeaderAcceleration();
}

void CollidableObject::joinIsland(std::shared_ptr<Island> new_island) {
    island->removeMember(*this);
    new_island->addMember(*this);
    island = new_island;
}

void CollidableObject::joinIslandAsLeader(std::shared_ptr<Island> new_island) {
    new_island->setLeader(*this);
    island = new_island;
}

void CollidableObject::joinIslandIfMovable(std::shared_ptr<Island> new_island) {
    if (type == CollidableObjectType::Movable) {
        joinIsland(std::move(new_island));
    }
}


std::shared_ptr<Island> CollidableObject::getIsland() const {
    return island;
}
