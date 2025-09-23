//
// Created by Agamjeet Singh on 21/09/25.
//

#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <string>

#include "geometry/Vector3f.h"

namespace eng::d3 {
    class PhysicsObject {
    public:
        /**
         * @brief Constructs the Entity with the given sprite and position. Sets the sprite to the given position.
         * @param position the entity's position upon construction.
         */
        explicit PhysicsObject(Vector3f position = {0, 0, 0});

        virtual ~PhysicsObject() = default;

        /**
         * @brief Maximum velocity of the gravity. This maximum is enforced by \code setGravityVelocity()\endcode and
         * \code addGravityVelocity()\endcode.
         */
        static constexpr float MAX_FALL = 240;

        /**
        * @brief The PhysicsObject's position.
        */
        Vector3f position;

        /**
         * @brief The base velocity of the object. It will always try to move with the base velocity.
         */
        Vector3f base_velocity = {0, 0, 0};

        /**
         * @brief The velocity imparted by the surface the object is resting on (if any) or if the object is a player
         * climbing another object.
         */
        Vector3f friction_velocity = {0, 0, 0};

        /**
         * @brief Returns the total velocity of the PhysicsObject. This is the object's true overall velocity.
         * @return The total velocity - sum of intrinsic, friction, impulse and gravitational velocities.
         */
        [[nodiscard]] Vector3f getTotalVelocity() const;

        Vector3f gravity_acceleration = {0, 0, 0};

        /**
         * @brief Prints the 3 velocities of the object with the given name in the format:
         * \code
         * name intrinsic_velocity: x = intrinsic_velocity.x, y = intrinsic_velocity.y, z = intrinsic_velocity.z
         * ...
         * \endcode
         * @param name The name that would appear on the print function
         */
        void printVelocity(const std::string& name) const;

        /**
         * @brief Prints the 3 velocities of the object with the address of the object as its name.
         */
        void printVelocity() const;

    private:
        /**
         * @return The address of the object in the form of a string.
         */
        std::string getAddressAsString() const;

        /**
         * @brief Velocity due to gravity. Is capped at \code MAX_FALL\endcode.
         */
        Vector3f gravity_velocity = {0, 0, 0};
    };
}



#endif //PHYSICSOBJECT_H
