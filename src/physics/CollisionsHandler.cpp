//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../include/physics/CollisionsHandler.h"

#include "../../include/events/Collision.h"
#include "../../include/physics/CollisionResolution.h"
#include "../../include/physics/ContactResolution.h"
#include "../../include/physics/ContactsHandler.h"

struct Collision;

std::size_t CollisionHash::operator()(const Collision& ref) const {
    return std::hash<void*>{}(&ref.objectA) ^
           std::hash<void*>{}(&ref.objectB) ^
           std::hash<float>{}(ref.collidingRectA.position.x) ^
           std::hash<float>{}(ref.collidingRectA.position.y) ^
           std::hash<float>{}(ref.collidingRectA.size.x) ^
           std::hash<float>{}(ref.collidingRectA.size.y) ^
           std::hash<float>{}(ref.collidingRectB.position.x) ^
           std::hash<float>{}(ref.collidingRectB.position.y) ^
           std::hash<float>{}(ref.collidingRectB.size.x) ^
           std::hash<float>{}(ref.collidingRectB.size.y) ^
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
    update(deltaTime, 0);
}

void CollisionsHandler::update(float deltaTime, int recursionDepth) {
    if (recursionDepth > 8) {
        return;  // Prevent infinite recursion
    }

    std::cout << ContactsHandler::getInstance().getContacts().size() << " contacts right now!!!!!!!!" << std::endl;
    if (!ContactsHandler::getInstance().getContacts().empty()) {

    }
    ContactsHandler::getInstance().reset();

    std::unordered_map<Collision, float, CollisionHash> collisions;
    for (auto bodyA: bodies) {
        for (auto bodyB: bodies) {
            // ~O(1) nested for loop, very few rectangles in every hitbox
            std::unordered_map<Collision, float, CollisionHash> body_ab_collisions;
            for (auto rectA: bodyA.get().getHitbox().getRects()) {
                for (auto rectB: bodyB.get().getHitbox().getRects()) {
                    auto incompleteCollision =
                        sweptCollision(
                            rectA,
                            rectB,
                            bodyA.get().velocity,
                            bodyB.get().velocity,
                            deltaTime);
                    if (&bodyA.get() > &bodyB.get() && incompleteCollision) {
                        body_ab_collisions[Collision{bodyA, bodyB, incompleteCollision.value()}] = incompleteCollision->collisionTime;
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
