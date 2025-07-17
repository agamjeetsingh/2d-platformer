//
// Created by Agamjeet Singh on 12/07/25.
//

#include "../../include/physics/Collider.h"

#include <iostream>
#include <unordered_set>

#include "../../include/physics/ColliderBodies.h"

class ColliderBodies;
Collider::Collider(std::vector<sf::FloatRect> hitbox) : position({0, 0}), hitbox(std::move(hitbox)) {
    ColliderBodies::getInstance().addBody(*this);
}

Collider::Collider(std::vector<sf::FloatRect> hitbox, sf::Vector2f position) : position(position), hitbox(std::move(hitbox)) {
    for (auto& box: this->hitbox) {
        box.position += position;
    }

    ColliderBodies::getInstance().addBody(*this);
}

[[nodiscard]] std::optional<Collider> Collider::setPosition(const sf::Vector2f position) {
    const sf::Vector2f old_position = this->position;
    const auto bodies = ColliderBodies::getInstance().getBodies();
    const auto old_hitbox = hitbox;
    const sf::Vector2f position_diff = position - old_position;
    
    this->position = position;
    
    for (auto& box: hitbox) {
        box.position += position_diff;
    }

    for (const auto& body: bodies) {
        if (collidesWith(body)) {
            // Reset the movement (TODO - or in future allow maximum possible movement before collision)
            this->position = old_position;

            for (auto& box: hitbox) {
                box.position -= position_diff;
            }
            return body;
        }
    }
    return std::nullopt;
}

// TODO - With my current implementation of setPosition and addPosition, if there are multiple collisions with a delta
// TODO - movement, I am returning an arbitrary one when I should return the "first" one.

[[nodiscard]] std::optional<Collider> Collider::addPosition(const sf::Vector2f position) {
    const sf::Vector2f old_position = this->position;
    const auto bodies = ColliderBodies::getInstance().getBodies();
    const auto old_hitbox = hitbox;
    this->position += position;

    for (auto& box: hitbox) {
        box.position += position;
    }

    for (const auto& body: bodies) {
        if (collidesWith(body)) {
            if (*this == body) {
                continue;
            }
            // Reset the movement (TODO - or in future allow maximum possible movement before collision)
            this->position = old_position;

            for (auto& box: hitbox) {
                box.position -= position;
            }
            assert(*this != body);
            return body;
        }
    }
    return std::nullopt;
}

[[nodiscard]] sf::Vector2f Collider::getPosition() const {
    return position;
}

[[nodiscard]] bool Collider::collidesWith(const Collider& other) const {
    for (const auto& this_box: hitbox) {
        for (const auto& other_box: other.getHitbox()) {
            if (this_box.findIntersection(other_box)) {
                return true;
            }
        }
    }
    return false;
}

struct FloatRectHash {
    std::size_t operator()(const sf::FloatRect& rect) const {
        return std::hash<float>{}(rect.position.x) ^
               std::hash<float>{}(rect.position.y) ^
               std::hash<float>{}(rect.size.x) ^
               std::hash<float>{}(rect.size.y);
    }
};

bool Collider::operator==(const Collider &other) const {
    if (hitbox == other.getHitbox()) {
        return position == other.position;
    }
    // Both hitboxes might still be the same with rectangles in different order
    const std::unordered_set<sf::FloatRect, FloatRectHash> this_hitbox_map(hitbox.begin(), hitbox.end());
    const std::unordered_set<sf::FloatRect, FloatRectHash> other_hitbox_map(other.getHitbox().begin(), other.getHitbox().end());
    return this_hitbox_map == other_hitbox_map && position == other.position;
}

[[nodiscard]] const std::vector<sf::FloatRect>& Collider::getHitbox() const {
    return hitbox;
}
