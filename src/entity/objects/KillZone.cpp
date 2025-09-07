//
// Created by Agamjeet Singh on 07/09/25.
//

#include "../../../include/entity/objects/KillZone.h"

KillZone::KillZone(sf::Vector2f position, sf::Vector2f size): CollidableObject({{{0, 0}, size}}, sf::Sprite{EmptyTextures::getInstance().getEmpty({1, 1})}, position),
                                                              collision_listener(Listener::make_listener<Collision>([this](const Collision& collision) {
                                                                  if (&collision.objectA != this && &collision.objectB != this) return;
                                                                  CollidableObject& other = &collision.objectA == this ? collision.objectB : collision.objectA;
                                                                  if (!other.isPlayer()) return;
                                                                  Player* player = other.isPlayer();
                                                                  player->kill();
                                                              })) {}

bool KillZone::canCollideWith(const CollidableObject &, Collision collision) const {
    return false;
}

