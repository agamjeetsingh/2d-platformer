//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef COLLIDERBODIES_H
#define COLLIDERBODIES_H
#include <unordered_set>
#include "Collider.h"

 /**
 * Hash function for \code std::reference_wrapper<Collider>\endcode
 */
struct ColliderRefHash {
    std::size_t operator()(const std::reference_wrapper<Collider>& ref) const {
        const Collider& collider = ref.get();
        return std::hash<float>{}(collider.getPosition().x) ^
               std::hash<float>{}(collider.getPosition().y) ^
               std::hash<unsigned long>{}(collider.getHitbox().size());
    }
};

/**
 * Equality function for \code std::reference_wrapper<Collider>\endcode
 */
struct ColliderRefEqual {
    bool operator()(const std::reference_wrapper<Collider>& lhs, const std::reference_wrapper<Collider>& rhs) const {
        return lhs.get() == rhs.get();
    }
};

class ColliderBodies {
public:
    /**
     * @brief gives the instance of ColliderBodies.
     * @return the singleton instance of ColliderBodies.
     */
    static ColliderBodies& getInstance();

    /**
     * @brief A const function that gives all the registered collider bodies.
     * @return the \code std::unordered_set\endcode that contains the reference to all the bodies.
     */
    [[nodiscard]] const std::unordered_set<std::reference_wrapper<Collider>, ColliderRefHash, ColliderRefEqual>& getBodies() const;

    /**
     * @brief Registers the body into the collider bodies unordered set.
     * @param body the body to be registered into the unordered set.
     */
    void addBody(Collider& body);

    /**
     * @brief Unregisters the given body from the collider bodies unordered set.
     * @param body the body to be unregistered from the unordered set.
     */
    void removeBody(Collider& body);

private:
    std::unordered_set<std::reference_wrapper<Collider>, ColliderRefHash, ColliderRefEqual> bodies;
};

#endif //COLLIDERBODIES_H
