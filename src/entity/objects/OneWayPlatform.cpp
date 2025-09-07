//
// Created by Agamjeet Singh on 06/09/25.
//

#include "../../../include/entity/objects/OneWayPlatform.h"

OneWayPlatform::OneWayPlatform(sf::Vector2f position): CollidableObject({{{0, 8}, {24, 2}}}, sf::Sprite{{EmptyTextures::getInstance().getEmpty({24, 16})}}, position, CollidableObjectType::Immovable) {
    assert(texture.loadFromFile("../assets/oneWayPlatform/wood_no_support.png"));
    sprite.setTexture(texture);
}

bool OneWayPlatform::canCollideWith(const CollidableObject &, Collision collision) const {
    const auto other_rect = collision.objectA == *this ? collision.getCollidingRectB() : collision.getCollidingRectA();
    const auto this_rect = collision.objectA == *this ? collision.getCollidingRectA() : collision.getCollidingRectB();
    constexpr float slop = 1;
    return other_rect.position.y + other_rect.size.y <= this_rect.position.y + slop;
}

