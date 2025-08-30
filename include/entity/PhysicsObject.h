//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <iostream>
#include <sstream>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>

class PhysicsObject {
public:
    /**
     * @brief Constructs the Entity with the given sprite and position. Sets the sprite to the given position.
     * @param sprite the sprite representing the entity.
     * @param position the entity's position upon construction.
     */
    explicit PhysicsObject(sf::Sprite sprite, sf::Vector2f position);

    virtual ~PhysicsObject() = default;

    /**
     * @brief Maximum velocity of the gravity. This maximum is enforced by \code setGravityVelocity()\endcode and
     * \code addGravityVelocity()\endcode.
     */
    static constexpr float MAX_FALL = 160;

    /**
     * @brief Returns a const reference to the position of the object. Useful for hitboxes.
     * @return A const reference to the position of the object.
     */
    const sf::Vector2f& getPositionRef() const;

    // Public to make it easy to update just one of the components
    /**
     * @brief The base velocity of the object. It will always try to move with the base velocity.
     */
    sf::Vector2f base_velocity = {0, 0};

    /**
     * @brief The velocity imparted by the surface the object is resting on (if any) or if the object is a player
     * climbing another object.
     */
    sf::Vector2f friction_velocity = {0, 0};

    /**
     * @brief Returns the total velocity of the PhysicsObject. This is the object's true overall velocity.
     * @return The total velocity - sum of intrinsic, friction, impulse and gravitational velocities.
     */
    [[nodiscard]] sf::Vector2f getTotalVelocity() const;

    sf::Vector2f gravity_acceleration = {0, 0};

    /**
     * @return A const reference to the sprite of the object.
     */
    [[nodiscard]] const sf::Sprite& getSprite() const;

    /**
     * @return The position of the object.
     */
    [[nodiscard]] sf::Vector2f getPosition() const;

    /**
     * @brief Sets the position to the given position. Also updates the position of the \code sprite\endcode.
     * @param position The new position of the object.
     */
    void setPosition(sf::Vector2f position);

    /**
     * @brief Adds the given position to the object's current position. Also updates the position of the \code sprite\endcode.
     * @param position The position to be added to the object's current position.
     */
    void addPosition(sf::Vector2f position);

    /**
     * @brief Prints the 3 velocities of the object with the given name in the format:
     * \code
     * name intrinsic_velocity: x = intrinsic_velocity.x, y = intrinsic_velocity.y
     * ...
     * \endcode
     * @param name The name that would appear on the print function
     */
    void printVelocity(const std::string& name) const;

    /**
     * @brief Prints the 3 velocities of the object with the address of the object as its name.
     */
    void printVelocity() const;

protected:
    sf::Sprite sprite;

private:
    /**
     * @brief The PhysicsObject's position.
     *
     * @note It is private because \code PhysicsObject\endcode handles updating the position of the sprite. Whenever a
     * position is updated, the sprite's position is updated as well using \code sprite.setPosition()\endcode.
     */
    sf::Vector2f position;

    /**
     * @return The address of the object in the form of a string.
     */
    std::string getAddressAsString() const;

    /**
     * @brief Velocity due to gravity. Is capped at
     */
    sf::Vector2f gravity_velocity = {0, 0};
};

#endif //PHYSICSOBJECT_H
