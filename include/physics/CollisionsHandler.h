//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef COLLIDERBODIES_H
#define COLLIDERBODIES_H
#include <unordered_set>
#include "CollisionAxis.h"
#include <SFML/Graphics.hpp>
#include "../entity/CollidableObject.h"

struct Collision;

struct IncompleteCollision {
    sf::FloatRect collidingRectA;
    sf::FloatRect collidingRectB;
    CollisionAxis axis;
    float deltaTime;
    float collisionTime;
};

struct CollisionHash {
    std::size_t operator()(const Collision& ref) const;
};

/**
 * Hash function for \code std::reference_wrapper<Collider>\endcode
 */
struct CollidableObjectRefHash {
    std::size_t operator()(const std::reference_wrapper<CollidableObject>& ref) const {
        const CollidableObject& collidable = ref.get();
        return std::hash<float>{}(collidable.getPosition().x) ^
               std::hash<float>{}(collidable.getPosition().y) ^
               std::hash<unsigned long>{}(collidable.getHitbox().getRects().size());
    }
};

/**c
 * Equality function for \code std::reference_wrapper<Collider>\endcode
 */
struct CollidableObjectRefEqual {
    bool operator()(const std::reference_wrapper<CollidableObject>& lhs, const std::reference_wrapper<CollidableObject>& rhs) const {
        return lhs.get() == rhs.get();
    }
};

class CollisionsHandler {
public:
    /**
     * @brief gives the instance of ColliderBodies.
     * @return the singleton instance of ColliderBodies.
     */
    static CollisionsHandler& getInstance();

    /**
     * @brief A const function that gives all the registered collider bodies.
     * @return the \code std::unordered_set\endcode that contains the reference to all the bodies.
     */
    [[nodiscard]] const std::unordered_set<std::reference_wrapper<CollidableObject>, CollidableObjectRefHash, CollidableObjectRefEqual>& getBodies() const;

    /**
     * @brief Registers the body into the collider bodies unordered set.
     * @param body the body to be registered into the unordered set.
     */
    void addObject(CollidableObject& body);

    /**
     * @brief Unregisters the given body from the collider bodies unordered set.
     * @param body the body to be unregistered from the unordered set.
     */
    void removeObject(CollidableObject& body);

    void update(float deltaTime);

    void drawHitboxes(sf::RenderWindow& window, sf::Color color = sf::Color::Red) const {
        color.a = 64;
        for (const auto& body: bodies) {
            for (const auto& box: body.get().getHitbox()) {
                sf::RectangleShape transparentRect(box.size);
                transparentRect.setPosition(box.position);
                transparentRect.setFillColor(color);
                window.draw(transparentRect);
            }
        }
    }

private:
    std::unordered_set<std::reference_wrapper<CollidableObject>, CollidableObjectRefHash, CollidableObjectRefEqual> bodies;

    void update(float deltaTime, int recursionDepth);

    static std::optional<IncompleteCollision> sweptCollision( // TODO - Should take in acceleration too with s = ut + 1/2at^2
        sf::FloatRect rectA,
        sf::FloatRect rectB,
        sf::Vector2f velocityA,
        sf::Vector2f velocityB,
        float deltaTime) {
        sf::Vector2f relative_velocity = velocityA - velocityB;
        float x_entry;
        float x_exit;
        float y_entry;
        float y_exit;
        CollisionAxis axis;
        if (abs(relative_velocity.x) < 1e-6f) {
            // No relative X movement - check if overlapping or aligned on X axis
            if (rectA.position.x < rectB.position.x + rectB.size.x && 
                rectA.position.x + rectA.size.x > rectB.position.x) {
                // Objects overlap or can collide on X axis
                x_entry = -std::numeric_limits<float>::infinity();  
                x_exit = std::numeric_limits<float>::infinity();
            } else {
                return std::nullopt;  // Will never collide on X axis
            }
        } else {
            if (relative_velocity.x > 0) {
                x_entry = (rectB.position.x - rectA.position.x - rectA.size.x) / relative_velocity.x;
                x_exit = x_entry + (rectA.size.x + rectB.size.x) / relative_velocity.x; // Wrong when x_entry < 0 ?
            } else {
                x_entry = (rectA.position.x - rectB.position.x - rectB.size.x) / abs(relative_velocity.x);
                x_exit = x_entry + (rectA.size.x + rectB.size.x) / abs(relative_velocity.x);
            }
        }

        if (abs(relative_velocity.y) < 1e-6f) {
            // No relative Y movement - check if overlapping or aligned on Y axis
            if (rectA.position.y < rectB.position.y + rectB.size.y && 
                rectA.position.y + rectA.size.y > rectB.position.y) {
                // Objects overlap or can collide on Y axis
                y_entry = -std::numeric_limits<float>::infinity();  
                y_exit = std::numeric_limits<float>::infinity();
            } else {
                return std::nullopt;  // Will never collide on Y axis
            }
        } else {
            if (relative_velocity.y > 0) {
                y_entry = (rectB.position.y - rectA.position.y - rectA.size.y) / relative_velocity.y;
                y_exit = y_entry + (rectA.size.y + rectB.size.y) / relative_velocity.y;
            } else {
                y_entry = (rectA.position.y - rectB.position.y - rectB.size.y) / abs(relative_velocity.y);
                y_exit = y_entry + (rectA.size.y + rectB.size.y) / abs(relative_velocity.y);
            }
        }

        float entry = std::max(x_entry, y_entry);
        float exit = std::min(x_exit, y_exit);
        if (entry > exit || entry < 0 || entry > deltaTime) {
            return std::nullopt;
        }

        // Determine collision direction based on which axis collision happens first
        if (x_entry > y_entry) {
            // X-axis collision - determine if hitting from left or right
            if (relative_velocity.x > 0) {
                axis = CollisionAxis::Left;  // rectA hitting rectB from the left
            } else {
                axis = CollisionAxis::Right;   // rectA hitting rectB from the right
            }
        } else {
            // Y-axis collision - determine if hitting from top or bottom
            if (relative_velocity.y > 0) {
                axis = CollisionAxis::Up;   // rectA hitting rectB from above
            } else {
                axis = CollisionAxis::Down;     // rectA hitting rectB from below
            }
        }

        return IncompleteCollision{rectA, rectB, axis, deltaTime, entry};
    }

    float resolveEarliestCollision(std::unordered_map<Collision, float, CollisionHash> collisions, float timeSpent = 0);
};

#endif //COLLIDERBODIES_H
