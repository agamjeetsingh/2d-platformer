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
    /**
     * @brief Constructor for CollidableObject. Adds the object to the \code CollisionsHandler\endcode .
     * Ensures the invariant that \code mass == 0\endcode if and only if type is \code CollidableObjectType::Immovable\endcode
     * since zero mass is our convention for infinite mass. Asserts that mass is non-negative (so all movables have a positive mass).
     * @param hitbox The hitbox of the CollidableObject
     * @param sprite The sprite of the CollidableObject
     * @param position The CollidableObject's initial position
     * @param type The CollidableObject's movability
     * @param mass The CollidableObject's mass
     * @note With the current implementation, setting the type to immovable but passing in a non-zero mass does NOT
     * crash the program. The mass simply gets overwritten to \code 0\endcode which might be confusing to the users of
     * this class. However, setting the type to movable with a non-positive mass WOULD crash the program.
     */
    CollidableObject(std::vector<sf::FloatRect> hitbox,
                     sf::Sprite sprite,
                     sf::Vector2f position = {0, 0},
                     CollidableObjectType type = CollidableObjectType::Movable,
                     float mass = 1);

    const CollidableObjectType type;
    const float mass;

    /**
     * Returns the mathematical inverse of the mass. Returns 0 if the object is immovable and has infinite mass.
     * @return The inverse of the mass.
     */
    [[nodiscard]] float getInvMass() const;

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

    [[nodiscard]] const Player* isPlayer() const;

    [[nodiscard]] virtual bool canCollideWith(const CollidableObject&, Collision collision) const;

protected:
    Hitbox hitbox;
};



#endif //COLLIDABLEOBJECT_H
