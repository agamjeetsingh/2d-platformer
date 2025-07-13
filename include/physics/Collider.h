//
// Created by Agamjeet Singh on 12/07/25.
//

#ifndef COLLIDER_H
#define COLLIDER_H
#include <vector>

#include "../../../../../../opt/homebrew/Cellar/sfml/3.0.1/include/SFML/Graphics/Rect.hpp"


class Collider {
public:
    /**
     * @brief Constructs the Collider object with the given hitbox.
     * @param hitbox The vector of rectangles that represent the hitbox of the collidable object. The position of the
     * top-left corner of the rectangles is relative to the top-left corner of the object.
     */
    explicit Collider(std::vector<sf::FloatRect> hitbox);

    /**
     * @brief Constructs the Collider object with the given hitbox and position.
     * @param hitbox The vector of rectangles that represent the hitbox of the collidable object. The position of the
     * top-left corner of the rectangles is relative to the top-left corner of the object.
     * @param position The position of the object. Note that the hitbox is relative the top-left corner of the object
     * which is its position.
     */
    Collider(std::vector<sf::FloatRect> hitbox, sf::Vector2f position);

    /**
     * @brief Sets the position of the object to the given one.
     * @param position The new position of the Collider object.
     * @return \code std::nullopt\endcode if the new position doesn't intersect with any other collider.
     * Otherwise, returns the collider its new position intersects with.
     */
    std::optional<Collider> setPosition(sf::Vector2f position);

    /**
     * @brief Updates the position of the object by adding the provided one to it.
     * @param position The position vector to be added.
     * @return \code std::nullopt\endcode if the new position doesn't intersect with any other collider.
     * Otherwise, returns the collider its new position intersects with.
     */
    std::optional<Collider> addPosition(sf::Vector2f position);

    /**
     * @brief Gets the position of the object.
     * @return The position vector of the object.
     */
    [[nodiscard]] sf::Vector2f getPosition() const;

    /**
     * @brief Checks whether the object collides with the other object by checking for intersections in their hitboxes.
     * @param other The other object to check collision with.
     * @return Returns true if and only if the object's and the other object's hitbox have a nonzero intersection.
     * This also means that it returns false if the objects are just touching.
     */
    [[nodiscard]] bool collidesWith(const Collider& other) const;

    bool operator==(const Collider& other) const;

    [[nodiscard]] const std::vector<sf::FloatRect>& getHitbox() const;

private:
    sf::Vector2f position;

    std::vector<sf::FloatRect> hitbox;
};

#endif //COLLIDER_H
