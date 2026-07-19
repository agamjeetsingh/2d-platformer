//
// Created by Agamjeet Singh on 07/09/25.
//

#include "../../../include/entity/objects/KillZone.h"

#include "events/EventBus.h"

KillZone::KillZone(EventBus& post_physics_bus, sf::Vector2f position, sf::Vector2f size, std::optional<std::string> path_to_texture): CollidableObject(
        {{{0, 0}, size}}, sf::Sprite{EmptyTextures::getInstance().getEmpty({1, 1})}, position),
    collision_listener(Listener::make_listener<Collision>(post_physics_bus, [this](const Collision &collision) {
        if (&collision.objectA != this && &collision.objectB != this) return;
        CollidableObject &other = &collision.objectA == this ? collision.objectB : collision.objectA;
        if (!other.isPlayer()) return;
        Player *player = other.isPlayer();
        player->kill();
    })), sprite(EmptyTextures::getInstance().getEmpty(sf::VideoMode::getDesktopMode().size)) {
    if (!path_to_texture) return;
    assert(texture.loadFromFile(path_to_texture.value()));
    sprite.setTexture(texture);
}

bool KillZone::canCollideWith(const CollidableObject &, Collision collision) const {
    return false;
}

