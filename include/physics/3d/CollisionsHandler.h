//
// Created by Agamjeet Singh on 21/09/25.
//

#ifndef COLLISIONSHANDLER_H
#define COLLISIONSHANDLER_H
#include <unordered_set>

#include "CollidableObject.h"
#include "SpacialHashMap.h"
#include "geometry/Box.h"
#include "geometry/Vector3f.h"

namespace eng::d3 {
    struct Collision;

    struct IncompleteCollision {
        Box collidingBoxA;
        Box collidingBoxB;
        Vector3f collision_axis;
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
            return std::hash<float>{}(collidable.position.x) ^
                   std::hash<float>{}(collidable.position.y) ^
                   std::hash<unsigned long>{}(collidable.getHitbox().getBoxes().size());
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
        CollisionsHandler();

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
         */
        void update(float deltaTime);

        // void drawHitboxes() const;
        // TODO - Draw OpenGL wireframe

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
         * @param boxA The first box.
         * @param boxB The second box.
         * @param axis The axis of the collision of the two rectangles.
         * @return the penetration along the axis between the two boxes or \code 0\endcode if
         * they don't intersect at all.
         */
        static float getPenetration(Box boxA, Box boxB, Vector3f axis);

        ContactsPtrHashMap buildContacts(float deltaTime);

        SpacialHashMap spacial_map;

        ContactsPtrHashMap next_frame_contacts;

        void buildSpatialMap();

        /**
         * @brief Gets the cell size for spatial hashing. It takes O(N) time where N is the number of bodies. Should only
         * be called when the \code bodies\endcode member has changed significantly.
         * @return the cell size for spatial hashing. Currently implemented as the maximum diameter of any body.
         */
        [[nodiscard]] float getNewCellSize() const;
    };
}



#endif //COLLISIONSHANDLER_H
