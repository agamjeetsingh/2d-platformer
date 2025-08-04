//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef COLLIDABLEENTITY_H
#define COLLIDABLEENTITY_H

#include <iostream>

#include "PhysicsObject.h"
#include "../physics/Hitbox.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "CollidableObjectType.h"

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

    /**
     * @brief Checks whether the object collides with the other object by checking for intersections in their hitboxes.
     * @param other The other object to check collision with.
     * @return Returns the intersecting \code sf::FloatRect\endcode if the object's and the other object's hitbox have
     * a nonzero intersection and returns \code std::nullopt\endcode otherwise. This also means that it returns
     * \code std::nullopt\endcode if the objects are just touching.
     */
    [[nodiscard]] std::optional<Collision> collidesWith(const CollidableObject& other) const;

private:
    Hitbox hitbox;
};



#endif //COLLIDABLEENTITY_H
