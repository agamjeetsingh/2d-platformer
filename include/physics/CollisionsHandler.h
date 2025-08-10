//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef COLLISIONSHANDLER_H
#define COLLISIONSHANDLER_H
#include <numeric>
#include <unordered_set>
#include "CollisionAxis.h"
#include <SFML/Graphics.hpp>

#include "SpacialHashMap.h"
#include "../entity/CollidableObject.h"

class Player;
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

/**
 * Equality function for \code std::reference_wrapper<Collider>\endcode
 */
struct CollidableObjectRefEqual {
    bool operator()(const std::reference_wrapper<CollidableObject>& lhs, const std::reference_wrapper<CollidableObject>& rhs) const {
        return lhs.get() == rhs.get();
    }
};

struct CollidableObjectPtrPairHash {
    std::size_t operator()(const std::pair<CollidableObject*, CollidableObject*>& pair) const {
        return std::hash<CollidableObject*>{}(pair.first) ^ (std::hash<CollidableObject*>{}(pair.second) << 1);
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

    void reset() {
        bodies = {};
    }

    void update(float deltaTime, Player& player);

    void drawHitboxes(sf::RenderWindow& window, sf::Color color = sf::Color::Red) const;

    [[nodiscard]] float getCellSize(bool update = false) const {
        static float cellSize = -1;
        if (cellSize == -1 || update) {
            cellSize = getNewCellSize();
        }
        return cellSize;
    }

private:
    std::unordered_set<std::reference_wrapper<CollidableObject>, CollidableObjectRefHash, CollidableObjectRefEqual> bodies;

    static float dot(const sf::Vector2f& first, const sf::Vector2f& second) {
        return first.x * second.x + first.y * second.y;
    }

    static float getPenetration(sf::FloatRect rectA, sf::FloatRect rectB, CollisionAxis axis);

    void moveImmovables(float deltaTime) const;

    void moveMovables(float deltaTime) const;

    std::unordered_map<std::pair<CollidableObject*, CollidableObject*>, Collision, CollidableObjectPtrPairHash> buildContacts(float deltaTime);

    std::unordered_map<std::pair<CollidableObject*, CollidableObject*>, Collision, CollidableObjectPtrPairHash> buildContactsFaster(float deltaTime);

    // TODO - Should take in acceleration too with s = ut + 1/2at^2
    static std::optional<IncompleteCollision> sweptCollision(
        sf::FloatRect rectA,
        sf::FloatRect rectB,
        sf::Vector2f velocityA,
        sf::Vector2f velocityB,
        float deltaTime);

    SpacialHashMap spacial_map;

    void buildSpatialMap();

    [[nodiscard]] float getNewCellSize() const;

};

#endif //COLLISIONSHANDLER_H
