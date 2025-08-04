//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../include/physics/CollisionsHandler.h"

#include <ranges>

#include "../../include/events/Collision.h"
#include "../../include/physics/CollisionResolution.h"
#include "../../include/physics/ContactResolution.h"
#include "../../include/physics/ContactsHandler.h"

struct Collision;

std::size_t CollisionHash::operator()(const Collision& ref) const {
    return std::hash<void*>{}(&ref.objectA) ^
           std::hash<void*>{}(&ref.objectB) ^
           std::hash<float>{}(ref.getCollidingRectA().position.x) ^
           std::hash<float>{}(ref.getCollidingRectA().position.y) ^
           std::hash<float>{}(ref.getCollidingRectA().size.x) ^
           std::hash<float>{}(ref.getCollidingRectA().size.y) ^
           std::hash<float>{}(ref.getCollidingRectB().position.x) ^
           std::hash<float>{}(ref.getCollidingRectB().position.y) ^
           std::hash<float>{}(ref.getCollidingRectB().size.x) ^
           std::hash<float>{}(ref.getCollidingRectB().size.y) ^
           std::hash<int>{}(static_cast<int>(ref.axis)) ^
           std::hash<float>{}(ref.deltaTime) ^
           std::hash<float>{}(ref.collisionTime);
}

CollisionsHandler& CollisionsHandler::getInstance() {
    static CollisionsHandler instance;
    return instance;
}

[[nodiscard]] const std::unordered_set<std::reference_wrapper<CollidableObject>, CollidableObjectRefHash, CollidableObjectRefEqual>& CollisionsHandler::getBodies() const {
    return bodies;
}

void CollisionsHandler::addObject(CollidableObject& body) {
    bodies.insert(std::ref(body));
}

void CollisionsHandler::removeObject(CollidableObject& body) {
    bodies.erase(std::ref(body));
}

void CollisionsHandler::update(float deltaTime) {
    std::unordered_map<std::pair<CollidableObject*, CollidableObject*>, Collision, CollidableObjectPtrPairHash> contacts;

    for (auto bodyA: bodies) {
        for (auto bodyB: bodies) {
            // ~O(1) nested for loop, very few rectangles in every hitbox
            std::unordered_map<Collision, float, CollisionHash> body_ab_collisions;
            for (std::size_t indexA = 0; indexA < bodyA.get().getHitbox().getRects().size(); indexA++) {
                auto rectA = bodyA.get().getHitbox().getRects()[indexA];
                for (std::size_t indexB = 0; indexB < bodyB.get().getHitbox().getRects().size(); indexB++) {
                    auto rectB = bodyB.get().getHitbox().getRects()[indexB];
                    auto incompleteCollision =
                        sweptCollision(
                            rectA,
                            rectB,
                            bodyA.get().velocity,
                            bodyB.get().velocity,
                            deltaTime);
                    if (&bodyA.get() > &bodyB.get() && incompleteCollision &&
                        !(bodyA.get().type == CollidableObjectType::Immovable && bodyB.get().type == CollidableObjectType::Immovable)) {
                        body_ab_collisions[Collision{bodyA, bodyB, incompleteCollision.value(), indexA, indexB}] = incompleteCollision->collisionTime;
                    }
                }
            }
            // Sort for earliest collision by time to get the sf::FloatRects and add it to the collisions map

            if (body_ab_collisions.empty()) {
                continue;
            }

            auto earliest_ab_it = std::min_element(body_ab_collisions.begin(), body_ab_collisions.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

            auto collision = earliest_ab_it->first;
            contacts.emplace(std::make_pair(&bodyA.get(), &bodyB.get()), earliest_ab_it->first);
        }
    }

    moveImmovables(deltaTime);
    moveMovables(deltaTime);

    for (int i = 0; i < 10; i++) {
        for (const auto &key: contacts | std::views::keys) {
            auto collision = contacts.at(key);
            auto normal = -axisToVector(collision.axis); // A moving towards B in this normal
            // Compute Penetration
            auto penetrationDepth = getPenetration(collision.getCollidingRectA(), collision.getCollidingRectB(), collision.axis);
            assert(penetrationDepth >= 0);

            auto objectA = key.first;
            auto objectB = key.second;

            sf::Vector2f relative_velocity = objectA->velocity - objectB->velocity;
            float relative_velocity_along_normal = dot(relative_velocity, normal);
            assert(relative_velocity_along_normal >= 0);

            float e = 0; // Zero restitution

            assert(objectA->getInvMass() + objectB->getInvMass() > 0);
            float j = -(1 + e) * relative_velocity_along_normal / (objectA->getInvMass() + objectB->getInvMass()); // Impulse

            sf::Vector2f impulse = j * normal;

            objectA->velocity += impulse * objectA->getInvMass();
            objectB->velocity -= impulse * objectB->getInvMass();

            // Positional correction
            float percent = 0.2;
            float slop = 0.01;
            float invMassSum = objectA->getInvMass() + objectB->getInvMass();
            assert(invMassSum > 0);
            sf::Vector2f correction = (std::max(penetrationDepth - slop, 0.0f) / invMassSum) * percent * normal;
            if (objectA->type != CollidableObjectType::Immovable) {
                objectA->addPosition(-correction * objectA->getInvMass());
            }
            if (objectB->type != CollidableObjectType::Immovable) {
                objectB->addPosition(correction * objectB->getInvMass());
            }
        }
    }

}


void CollisionsHandler::moveImmovables(float deltaTime) const {

    if (deltaTime <= 0) {
        return;
    }

    // TODO - Make a array keeping track of all immovables as a private member of the class
    std::vector<std::reference_wrapper<CollidableObject>> immovables;
    for (auto body: bodies) {
        if (body.get().type == CollidableObjectType::Immovable) {
            immovables.push_back(body);
        }
    }

    std::unordered_map<Collision, float, CollisionHash> collisions;

    for (auto bodyA: immovables) {
        for (auto bodyB: immovables) {
            // ~O(1) nested for loop, very few rectangles in every hitbox
            std::unordered_map<Collision, float, CollisionHash> body_ab_collisions;
            for (std::size_t indexA = 0; indexA < bodyA.get().getHitbox().getRects().size(); indexA++) {
                auto rectA = bodyA.get().getHitbox().getRects()[indexA];
                for (std::size_t indexB = 0; indexB < bodyB.get().getHitbox().getRects().size(); indexB++) {
                    auto rectB = bodyB.get().getHitbox().getRects()[indexB];
                    auto incompleteCollision =
                        sweptCollision(
                            rectA,
                            rectB,
                            bodyA.get().velocity,
                            bodyB.get().velocity,
                            deltaTime);
                    if (&bodyA.get() > &bodyB.get() && incompleteCollision) {
                        body_ab_collisions[Collision{bodyA, bodyB, incompleteCollision.value(), indexA, indexB}] = incompleteCollision->collisionTime;
                    }
                }
            }

            // Sort for earliest collision by time to get the sf::FloatRects and add it to the collisions map

            if (body_ab_collisions.empty()) {
                continue;
            }

            auto earliest_ab_it = std::min_element(body_ab_collisions.begin(), body_ab_collisions.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

            collisions[earliest_ab_it->first] = earliest_ab_it->second;
        }
    }

    if (collisions.empty()) {
        for (auto body: immovables) {
            body.get().velocity += body.get().acceleration * deltaTime;
            body.get().addPosition(body.get().velocity * deltaTime);
        }
        return;
    }

    auto earliest_it = std::min_element(collisions.begin(), collisions.end(),
    [](const auto& a, const auto& b) { return a.second < b.second; });

    const Collision& earliest_collision = earliest_it->first;


    // Move all objects to earliest_collision.collisionTime

    for (auto body: immovables) {
        body.get().velocity += body.get().acceleration * earliest_collision.collisionTime;
        body.get().addPosition(body.get().velocity * earliest_collision.collisionTime);
    }

    earliest_collision.objectA.velocity = {0, 0};
    earliest_collision.objectA.acceleration = {0, 0};
    earliest_collision.objectB.velocity = {0, 0};
    earliest_collision.objectB.acceleration = {0, 0};

    moveImmovables(deltaTime - earliest_collision.collisionTime);
}

void CollisionsHandler::moveMovables(float deltaTime) const {
    for (auto body: bodies) {
        if (body.get().type == CollidableObjectType::Movable) {
            body.get().velocity += body.get().acceleration * deltaTime;
            body.get().addPosition(body.get().velocity * deltaTime);
        }
    }
}

void CollisionsHandler::update(float deltaTime, int recursionDepth) {
    if (recursionDepth > 8) {
        return;  // Prevent infinite recursion
    }

    std::unordered_map<Collision, float, CollisionHash> collisions;
    for (auto bodyA: bodies) {
        for (auto bodyB: bodies) {
            // ~O(1) nested for loop, very few rectangles in every hitbox
            std::unordered_map<Collision, float, CollisionHash> body_ab_collisions;
            for (std::size_t indexA = 0; indexA < bodyA.get().getHitbox().getRects().size(); indexA++) {
                auto rectA = bodyA.get().getHitbox().getRects()[indexA];
                for (std::size_t indexB = 0; indexB < bodyB.get().getHitbox().getRects().size(); indexB++) {
                    auto rectB = bodyB.get().getHitbox().getRects()[indexB];
                    auto incompleteCollision =
                        sweptCollision(
                            rectA,
                            rectB,
                            bodyA.get().velocity,
                            bodyB.get().velocity,
                            deltaTime);
                    if (&bodyA.get() > &bodyB.get() && incompleteCollision) {
                        body_ab_collisions[Collision{bodyA, bodyB, incompleteCollision.value(), indexA, indexB}] = incompleteCollision->collisionTime;
                    }
                }
            }
            // Sort for earliest collision by time to get the sf::FloatRects and add it to the collisions map

            if (body_ab_collisions.empty()) {
                continue;
            }

            auto earliest_ab_it = std::min_element(body_ab_collisions.begin(), body_ab_collisions.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

            collisions[earliest_ab_it->first] = earliest_ab_it->second;
        }
    }

    if (collisions.empty()) {
        // No collisions

        std::cout << "No collision :)" << std::endl;

        // Update velocities, accelerations for deltaTime
        // Use acceleration too s = ut + 1/2 at^2
        for (auto body: bodies) {
            body.get().addPosition(body.get().velocity * deltaTime);
            body.get().velocity += body.get().acceleration * deltaTime;
        }

        assert(ContactsHandler::getInstance().getContacts().empty());
        // ContactsHandler::getInstance().updateIslands();

        return;
    }

    // TODO - REMOVE LATER
    std::cout << "Collision!!!" << deltaTime << std::endl;

    // // Sort for earliest collision by time and resolve it first, then call update again deltaTime-collisionTime                                                                                   │ │
    // auto earliest_it = std::min_element(collisions.begin(), collisions.end(),
    //     [](const auto& a, const auto& b) { return a.second < b.second; });
    //
    // Collision earliest_collision = earliest_it->first;
    //
    // std::cout << "earliest_collision.collisionTime = " << earliest_collision.collisionTime << std::endl;
    //
    // // Update velocities, accelerations for earliest_collision.collisionTime
    // for (auto body: bodies) {
    //     // Update position first because the logic is based on current velocity
    //     body.get().addPosition(body.get().velocity * earliest_collision.collisionTime);
    //     body.get().velocity += body.get().acceleration * earliest_collision.collisionTime;
    // }
    //
    // // Resolve collisions
    // CollisionResolution::resolve(earliest_collision);

    auto earliest_collision_time = resolveEarliestCollision(collisions);

    if (deltaTime > earliest_collision_time) {
        update(deltaTime - earliest_collision_time, recursionDepth + 1);
    }

    // TODO - limit the number of update calls to something like 8N where N is the number of original collisions
}

float CollisionsHandler::resolveEarliestCollision(std::unordered_map<Collision, float, CollisionHash> collisions, float timeSpent) {
    // Sort for earliest collision by time and resolve it first, then call update again deltaTime-collisionTime                                                                                   │ │
    auto earliest_it = std::min_element(collisions.begin(), collisions.end(),
    [](const auto& a, const auto& b) { return a.second < b.second; });

    const Collision& earliest_collision = earliest_it->first;

    std::cout << "Collision time: " << earliest_collision.collisionTime << std::endl;

    if (earliest_collision.collisionTime < 1e-6) {
        // Contact

        ContactsHandler::getInstance().addContact(Contact(earliest_collision));
        if (!(earliest_collision.collisionTime == 0)) {
            std::cout << "!!! Treating collision time of " << earliest_collision.collisionTime << "s as contact" << std::endl;

            // Update velocities, accelerations for earliest_collision.collisionTime (very small time)
            for (auto body: bodies) {
                // Update position first because the logic is based on current velocity
                body.get().addPosition(body.get().velocity * earliest_collision.collisionTime);
                body.get().velocity += body.get().acceleration * earliest_collision.collisionTime;
            }

            timeSpent += earliest_collision.collisionTime;
        }

        // TODO - Resolve contact
        ContactResolution::resolve(earliest_collision);

        const auto erased = collisions.erase(earliest_collision);
        assert(erased);

        return collisions.empty() ? timeSpent : resolveEarliestCollision(collisions, timeSpent);
    }

    // Collision (not a contact)

    // Update velocities, accelerations for earliest_collision.collisionTime
    for (auto body: bodies) {
        // Update position first because the logic is based on current velocity
        body.get().addPosition(body.get().velocity * earliest_collision.collisionTime);
        body.get().velocity += body.get().acceleration * earliest_collision.collisionTime;
    }
    timeSpent += earliest_collision.collisionTime;

    // Resolve collisions
    CollisionResolution::resolve(earliest_collision);

    return timeSpent;
}
