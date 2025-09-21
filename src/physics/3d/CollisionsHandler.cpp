//
// Created by Agamjeet Singh on 21/09/25.
//

#include "../../../include/physics/3d/CollisionsHandler.h"

#include <cassert>
#include <numeric>

#include "../../../include/physics/3d/Collision.h"
#include "../../../include/physics/3d/CollidableObject.h"

#include <ranges>

#include "events/EventBus.h"

std::size_t eng::d3::CollisionHash::operator()(const Collision& ref) const {
    return std::hash<void*>{}(&ref.objectA) ^
           std::hash<void*>{}(&ref.objectB) ^
           std::hash<float>{}(ref.getCollidingBoxA().position.x) ^
           std::hash<float>{}(ref.getCollidingBoxA().position.y) ^
           std::hash<float>{}(ref.getCollidingBoxA().size.x) ^
           std::hash<float>{}(ref.getCollidingBoxA().size.y) ^
           std::hash<float>{}(ref.getCollidingBoxB().position.x) ^
           std::hash<float>{}(ref.getCollidingBoxB().position.y) ^
           std::hash<float>{}(ref.getCollidingBoxB().size.x) ^
           std::hash<float>{}(ref.getCollidingBoxB().size.y) ^
           std::hash<float>{}(ref.deltaTime) ^
           std::hash<float>{}(ref.collisionTime);
}

eng::d3::CollisionsHandler&eng::d3::CollisionsHandler::getInstance() {
    static CollisionsHandler instance;
    return instance;
}

[[nodiscard]] const std::unordered_set<std::reference_wrapper<eng::d3::CollidableObject>,
    eng::d3::CollidableObjectRefHash, eng::d3::CollidableObjectRefEqual> &eng::d3::CollisionsHandler::getBodies() const {
    return bodies;
}

void eng::d3::CollisionsHandler::addObject(CollidableObject& body) {
    bodies.insert(std::ref(body));
}

void eng::d3::CollisionsHandler::removeObject(CollidableObject& body) {
    bodies.erase(std::ref(body));
}

void eng::d3::CollisionsHandler::update(float deltaTime) {

    std::unordered_set<CollidableObject*> friction_set_bodies;

    for (int i = 0; i < 8; i++) {
        for (const auto& key: next_frame_contacts | std::views::keys) {
            auto collision = next_frame_contacts.at(key);
            auto normal = collision.axis; // A moving towards B in this normal
            // Compute Penetration
            auto penetrationDepth = getPenetration(collision.getCollidingBoxB(), collision.getCollidingBoxB(), collision.axis);
            assert(penetrationDepth >= 0);

            auto objectA = key.first;
            auto objectB = key.second;

            // ==== Normal Impulse ====
            Vector3f relative_velocity = objectA->getTotalVelocity() - objectB->getTotalVelocity();
            float relative_velocity_along_normal = relative_velocity.dot(normal);

            if (relative_velocity_along_normal < 0) {
                continue;
            }

            float e = 0; // Zero restitution

            assert(objectA->getInvMass() + objectB->getInvMass() > 0);
            float j = -(1 + e) * relative_velocity_along_normal / (objectA->getInvMass() + objectB->getInvMass());
            // Impulse

            Vector3f impulse = j * normal;

            // ==== Friction Impulse ====
            if (i == 0 && (objectA->type == CollidableObjectType::Immovable || objectB->type ==
                           CollidableObjectType::Immovable)) {
                auto *immovable = objectA->type == CollidableObjectType::Immovable ? objectA : objectB;
                auto *movable = immovable == objectA ? objectB : objectA;
                bool friction_set = false;
                if (collision.axis.x != 0) {
                    if (relative_velocity.dot(normal) > 0) {
                        movable->friction_velocity.y = immovable->getTotalVelocity().y;
                        movable->friction_velocity.z = immovable->getTotalVelocity().z;
                        friction_set = true;
                    }
                }
                if (collision.axis.y != 0) {
                    if (relative_velocity.dot(normal) > 0) {
                        movable->friction_velocity.x = immovable->getTotalVelocity().x;
                        movable->friction_velocity.z = immovable->getTotalVelocity().z;
                        friction_set = true;
                    }
                }
                if (collision.axis.z != 0) {
                    if (relative_velocity.dot(normal) > 0) {
                        movable->friction_velocity.x = immovable->getTotalVelocity().x;
                        movable->friction_velocity.y = immovable->getTotalVelocity().y;
                        friction_set = true;
                    }
                }
                if (friction_set) {
                    friction_set_bodies.insert(movable);
                }
            }

            objectA->base_velocity += impulse * objectA->getInvMass();
            objectB->base_velocity -= impulse * objectB->getInvMass();

            float invMassSum = objectA->getInvMass() + objectB->getInvMass();
            float percent = 0.2;
            float slop = 0.01;
            assert(invMassSum > 0);
            Vector3f correction = (std::max(penetrationDepth - slop, 0.0f) / invMassSum) * percent * normal;
            if (objectA->type != CollidableObjectType::Immovable) {
                if (objectB->type == CollidableObjectType::Immovable) {
                    objectA->position += -std::max(penetrationDepth - slop, 0.0f) * normal;
                } else {
                    objectA->position += -correction * objectA->getInvMass();
                }
            }
            if (objectB->type != CollidableObjectType::Immovable) {
                if (objectA->type == CollidableObjectType::Immovable) {
                    objectB->position += std::max(penetrationDepth - slop, 0.0f) * normal;
                } else {
                    objectB->position += correction * objectB->getInvMass();
                }
            }
        }
    }

    for (const auto &body: bodies) {
        if (!friction_set_bodies.contains(&body.get())) {
            body.get().friction_velocity = {0, 0, 0};
        }
    }

    next_frame_contacts = buildContacts(deltaTime);

    for (auto body: bodies) {
        body.get().base_velocity += body.get().gravity_acceleration * deltaTime;
        body.get().base_velocity.y = std::min(CollidableObject::MAX_FALL, body.get().base_velocity.y);
        body.get().base_velocity += body.get().getTotalVelocity() * deltaTime;
    }
}

eng::d3::ContactsPtrHashMap eng::d3::CollisionsHandler::buildContacts(float deltaTime) {
    ContactsPtrHashMap contacts;
    std::vector<Collision> phantom_collisions;

    buildSpatialMap();

    auto pairs = spacial_map.getPairs();

    for (auto [bodyA, bodyB]: pairs) {
        size_t indexA = 0;
        for (const auto& boxA : bodyA->getHitbox()) {
            size_t indexB = 0;
            for (const auto& boxB : bodyB->getHitbox()) {
                if (auto intersection = boxA.findIntersection(boxB)) {
                    auto size = intersection.value().size;
                    Vector3f axis;
                    const float min = std::min(size.x, std::min(size.y, size.z));
                    if (size.y == min) {
                        axis = (boxA.position.y > boxB.position.y) ? Vector3f{0, 1, 0} : Vector3f{0, -1, 0};
                    }
                    if (size.x == min) {
                        if (size.x < 0.01) continue;
                        axis = (boxA.position.x > boxB.position.x) ? Vector3f{1, 0, 0} : Vector3f{-1, 0, 0};
                    }
                    if (size.z == min) {
                        if (size.z < 0.01) continue;
                        axis = (boxA.position.x > boxB.position.x) ? Vector3f{0, 0, 1} : Vector3f{0, 0, 1};
                    }
                    const Collision collision = {*bodyA, *bodyB, IncompleteCollision{boxA, boxB, axis, deltaTime, deltaTime}, indexA, indexB};
                    if (!bodyA->canCollideWith(*bodyB, collision) || !bodyB->canCollideWith(*bodyA, collision)) {
                        phantom_collisions.push_back(collision);
                    } else {
                        contacts.emplace(std::make_pair(bodyA, bodyB), collision);
                    }
                }
                indexB++;
            }
            indexA++;
        }
    }

    for (const auto& collision: phantom_collisions) {
        EventBus::getInstance().emit(collision, EventExecuteTime::POST_PHYSICS);
    }

    // ContactsHandler::getInstance().newFrame();
    for (const auto& contact: contacts | std::ranges::views::values) {
        // ContactsHandler::getInstance().addContact(Contact(contact));
    }

    // ContactsHandler::getInstance().emitPlayerEvents();

    return contacts;
}



float eng::d3::CollisionsHandler::getPenetration(Box boxA, Box boxB, Vector3f axis) {
    if (axis.x != 0) {
        if (auto intersection = boxA.findIntersection(boxB)) {
            return intersection.value().size.x;
        } else {
            return 0;
        }
    }

    if (axis.y != 0) {
        if (auto intersection = boxA.findIntersection(boxB)) {
            return intersection.value().size.y;
        } else {
            return 0;
        }
    }

    if (axis.z != 0) {
        if (auto intersection = boxA.findIntersection(boxB)) {
            return intersection.value().size.z;
        } else {
            return 0;
        }
    }

    return 0;
}

void eng::d3::CollisionsHandler::buildSpatialMap() {
    auto updated = getCellSize(true);

    if (bodies.size() != spacial_map.getNumObjects()) {
        spacial_map = SpacialHashMap{bodies.size()};
    } else {
        spacial_map.clear();
    }


    for (const auto& body: bodies) {
        spacial_map.addObject(&body.get());
    }
}

float eng::d3::CollisionsHandler::getCellSize(bool update) const {
    static float cellSize = -1;
    if (cellSize == -1 || update) {
        cellSize = getNewCellSize();
    }
    return cellSize;
}


float eng::d3::CollisionsHandler::getNewCellSize() const {
    std::vector<float> diameters;
    diameters.reserve(bodies.size());
    for (auto body: bodies) {
        Vector3f size = body.get().getHitbox().getBounds().size;
        diameters.push_back(std::max(size.x, size.y));
    }
    // return *std::ranges::max_element(diameters);
    return std::accumulate(diameters.begin(), diameters.end(), 0.f) / static_cast<float>(bodies.size());
}
