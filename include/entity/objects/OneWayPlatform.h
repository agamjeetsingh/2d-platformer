//
// Created by Agamjeet Singh on 06/09/25.
//

#ifndef ONEWAYPLATFORM_H
#define ONEWAYPLATFORM_H
#include "entity/CollidableObject.h"
#include "events/Collision.h"
#include "utility/EmptyTextures.h"


class OneWayPlatform final : public CollidableObject {
public:
    explicit OneWayPlatform(sf::Vector2f position);

    bool canCollideWith(const CollidableObject&, Collision collision) const override;

private:
    sf::Texture texture;
};



#endif //ONEWAYPLATFORM_H
