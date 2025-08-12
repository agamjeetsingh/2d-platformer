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

using ContactsPtrHashMap = std::unordered_map<std::pair<CollidableObject*, CollidableObject*>, Collision, CollidableObjectPtrPairHash>;
using BodiesHashTable = std::unordered_set<std::reference_wrapper<CollidableObject>, CollidableObjectRefHash, CollidableObjectRefEqual>;

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

    /**
     * @brief Resets the CollisionsHandler by removing all the bodies registered in it.
     */
    void reset() {
        bodies = {};
    }

    /**
     * @brief Updates the whole physics by the given time, including the player inputs.
     * First builds the list of contacts according to predicted position in deltaTime, moves the immovable objects
     * according to swept collision, then moves the movable objects. Then iteratively does impulse based resolution of
     * all the contacts. Resets friction to zero for objects no longer resting on a surface.
     *
     * @param deltaTime The time (in seconds) to update the physics by
     * @param player The player whose inputs are to be handled
     */
    void update(float deltaTime, Player& player);

    /**
     * @brief Draws the hitbox of each registered body to the given window with the given color with an alpha value of
     * \code 64\endcode.
     * @param window The window to draw the hitboxes in.
     * @param color The color of the hitbox, red by default.
     */
    void drawHitboxes(sf::RenderWindow& window, sf::Color color = sf::Color::Red) const;

    /**
     * @brief Gets the cell size of the spatial hash. Updating the cell size takes O(N) time where N is the number of
     * bodies. Cell size should only be updated when the \code bodies\endcode member has changed significantly.
     * @param update Whether to get a new cell size according to the current bodies or not
     * @return The cell size, an updated one if \code update\endcode was true.
     */
    [[nodiscard]] float getCellSize(bool update = false) const;

private:
    /**
     * @brief The hash table that stores all the bodies.
     */
    BodiesHashTable bodies;

    /**
     * @brief Gets the penetration of two rectangles in a collision. Used in positional correction.
     * @param rectA The first rectangle.
     * @param rectB The second rectangle.
     * @param axis The axis of the collision of the two rectangles.
     * @return the penetration along the axis (Vertical or Horizontal) between the two rectangles or \code 0\endcode if
     * they don't intersect at all.
     */
    static float getPenetration(sf::FloatRect rectA, sf::FloatRect rectB, CollisionAxis axis);

    /**
     * @brief Moves all the immovable objects by the given amount of time with swept collision since impulse based
     * resolution would not work on two objects of infinite mass. Upon collision, both object's velocity and
     * acceleration are set to zero.
     * @param deltaTime The time (in seconds) to move the immovable objects by.
     */
    void moveImmovables(float deltaTime) const;

    /**
     * @brief Moves all the movable objects by the given amount of time without any swept collision. Allows
     * penetration which would later be dealt with by impulse resolution.
     * @param deltaTime The time (in seconds) to move the movable objects by.
     */
    void moveMovables(float deltaTime) const;

    // TODO - remove swept collision at all from buildContacts and buildContactsFaster and put it after moveMovables()
    // and moveImmovables()
    /**
     * @brief
     * @param deltaTime Builds the contacts hash map
     * @return The list of contacts that will be happening in deltaTime.
     */
    [[deprecated("Use buildContactsFaster() instead since it uses spatial hashing")]]
    ContactsPtrHashMap buildContacts(float deltaTime) const;

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

    /**
     * @brief Gets the cell size for spatial hashing. It takes O(N) time where N is the number of bodies. Should only
     * be called when the \code bodies\endcode member has changed significantly.
     * @return the cell size for spatial hashing. Currently implemented as the maximum diameter of any body.
     */
    [[nodiscard]] float getNewCellSize() const;

};

#endif //COLLISIONSHANDLER_H
