//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <iostream>
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

    const sf::Vector2f& getPositionRef() const { return position; }

    // Public to make it easy to update just one of the components
    sf::Vector2f intrinsic_velocity = {0, 0};

    sf::Vector2f friction_velocity = {0, 0};

    sf::Vector2f impulse_velocity = {0, 0};

    [[nodiscard]] sf::Vector2f getTotalVelocity() const;

    sf::Vector2f acceleration = {0, 0};

    virtual void update(float deltaTime);

    [[nodiscard]] const sf::Sprite& getSprite() const;

    [[nodiscard]] sf::Vector2f getPosition() const;

    void setPosition(sf::Vector2f position);

    void addPosition(sf::Vector2f position);

    void printVelocity(const std::string& name) const {
        std::cout << name << " intrinsic_velocity: x = " << intrinsic_velocity.x << " y = " << intrinsic_velocity.y << std::endl;
        std::cout << name << " friction_velocity: x = " << friction_velocity.x << " y = " << friction_velocity.y << std::endl;
        std::cout << name << " impulse_velocity: x = " << impulse_velocity.x << " y = " << impulse_velocity.y << std::endl;
    }

protected:
    sf::Sprite sprite;

private:
    sf::Vector2f position;
};

#endif //PHYSICSOBJECT_H
