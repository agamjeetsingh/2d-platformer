//
// Created by Agamjeet Singh on 14/09/25.
//

#ifndef PORTAL_H
#define PORTAL_H
#include "../../physics/2d/CollidableObject.h"
#include "utility/EmptyTextures.h"
#include <SFML/Graphics.hpp>

#include "../../physics/2d/Collision.h"

class Portal final : public CollidableObject {
    Portal(): CollidableObject({{{0, 0}, {24, 24}}}, sf::Sprite{EmptyTextures::getInstance().getEmpty({24, 24})}) {}

    void updateSprite(float deltaTime) {

    }

    bool canCollideWith(const CollidableObject &, Collision collision) const override {
        return false;
    }
};



#endif //PORTAL_H
