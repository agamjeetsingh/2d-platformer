//
// Created by Agamjeet Singh on 07/09/25.
//

#ifndef KILLZONE_H
#define KILLZONE_H
#include "entity/CollidableObject.h"
#include "entity/player/Player.h"
#include "events/Collision.h"
#include "events/Listener.h"
#include "utility/EmptyTextures.h"


class KillZone final : public CollidableObject {
public:
    explicit KillZone(sf::Vector2f position = {0, 0}, sf::Vector2f size = {0, 0}, std::optional<std::string> path_to_texture = std::nullopt);

    bool canCollideWith(const CollidableObject &, Collision collision) const override;

    const sf::Sprite* getSprite() const {
        return &sprite;
    }

private:
    Listener collision_listener;
    sf::Texture texture;
    sf::Sprite sprite;
};



#endif //KILLZONE_H
