//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef COLLIDABLEOBJECT_H
#define COLLIDABLEOBJECT_H

#include <iostream>

#include "PhysicsObject.h"
#include "../physics/Hitbox.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "CollidableObjectType.h"
class Player;

struct Collision;

class CollidableObject : public PhysicsObject {
public:
    CollidableObject(std::vector<sf::FloatRect> hitbox,
        sf::Sprite sprite,
        sf::Vector2f position = {0, 0},
        CollidableObjectType type = CollidableObjectType::Movable,
        float mass = 1);

    const CollidableObjectType type;
    const float mass;

    [[nodiscard]] float getInvMass() const {
        return (mass == 0) ? 0 : (1.0f / mass);
    }

    /**
 * @brief Gives the hitbox of the collider.
 * @return the hitbox of the collider.
 */
    [[nodiscard]] const Hitbox& getHitbox() const;

    /**
     * @brief Checks whether our collider is equal to another collider.
     * @param other The other collider we are comparing with.
     * @return true if and only if both colliders are the same object in memory. This is enough because it is mainly
     * used to avoid self collisions.
     */
    bool operator==(const CollidableObject& other) const;

    [[nodiscard]] Player* isPlayer();

private:
    Hitbox hitbox;
};



#endif //COLLIDABLEOBJECT_H
