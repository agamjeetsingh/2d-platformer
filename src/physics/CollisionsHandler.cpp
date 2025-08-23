//
// Created by Agamjeet Singh on 13/07/25.
//

#include "physics/CollisionsHandler.h"

#include <ranges>

#include "events/Collision.h"
#include "physics/ContactResolution.h"
#include "physics/ContactsHandler.h"
#include "entity/player/PlayerInputHandler.h"

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

void CollisionsHandler::update(float deltaTime, Player& player) {

    auto contacts = buildContactsFaster(deltaTime);

    moveImmovables(deltaTime);
    moveMovables(deltaTime);

    for (const auto& body: bodies) {
        constexpr float lambda = 10;
        // body.get().impulse_velocity *= std::exp(-lambda * deltaTime);
        body.get().impulse_velocity *= 0.0f;
        // if (body.get().impulse_velocity.x < 0.1) {
        //     body.get().impulse_velocity.x = 0;
        // }
        // if (body.get().impulse_velocity.y < 0.1) {
        //     body.get().impulse_velocity.y = 0;
        // }
    }

    PlayerInputHandler{player}.update(deltaTime);

    std::unordered_set<CollidableObject*> friction_set_bodies;

    for (int i = 0; i < 8; i++) {
        for (const auto& key: contacts | std::views::keys) {
            auto collision = contacts.at(key);
            auto normal = -axisToVector(collision.axis); // A moving towards B in this normal
            // Compute Penetration
            auto penetrationDepth = getPenetration(collision.getCollidingRectA(), collision.getCollidingRectB(),
                                                   collision.axis);
            assert(penetrationDepth >= 0);

            auto objectA = key.first;
            auto objectB = key.second;

            // ==== Normal Impulse ====
            sf::Vector2f relative_velocity = objectA->getTotalVelocity() - objectB->getTotalVelocity();
            float relative_velocity_along_normal = relative_velocity.dot(normal);

            if (relative_velocity_along_normal < 0) {
                continue;
            }

            float e = 0; // Zero restitution

            assert(objectA->getInvMass() + objectB->getInvMass() > 0);
            float j = -(1 + e) * relative_velocity_along_normal / (objectA->getInvMass() + objectB->getInvMass());
            // Impulse

            sf::Vector2f impulse = j * normal;

            // ==== Friction Impulse ====
            if (i == 0 && (objectA->type == CollidableObjectType::Immovable || objectB->type ==
                           CollidableObjectType::Immovable)) {
                auto *immovable = objectA->type == CollidableObjectType::Immovable ? objectA : objectB;
                auto *movable = immovable == objectA ? objectB : objectA;
                bool friction_set = false;
                if (collision.axis == CollisionAxis::Up || collision.axis == CollisionAxis::Down) {
                    if (movable->getTotalVelocity().dot(normal) > 0) {
                        movable->friction_velocity.x = immovable->getTotalVelocity().x;
                        friction_set = true;
                    }
                } else {
                    if (movable->getTotalVelocity().dot(normal) > 0) {
                        movable->friction_velocity.y = immovable->getTotalVelocity().y;
                        friction_set = true;
                    }
                }
                if (friction_set) {
                    friction_set_bodies.insert(movable);
                }
            }

            objectA->impulse_velocity += impulse * objectA->getInvMass();
            objectB->impulse_velocity -= impulse * objectB->getInvMass();

            float invMassSum = objectA->getInvMass() + objectB->getInvMass();
            float percent = 0.2;
            float slop = 0.01;
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

    for (const auto &body: bodies) {
        if (!friction_set_bodies.contains(&body.get())) {
            body.get().friction_velocity = {0, 0};
        }
    }
}


void CollisionsHandler::moveImmovables(float deltaTime) const {
    if (deltaTime <= 0) {
        return;
    }

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
                    if (&bodyA.get() <= &bodyB.get()) {
                        continue;
                    }
                    auto incompleteCollision =
                        sweptCollision(
                            rectA,
                            rectB,
                            bodyA.get().getTotalVelocity(),
                            bodyB.get().getTotalVelocity(),
                            deltaTime);
                    if (incompleteCollision) {
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
            body.get().impulse_velocity += body.get().acceleration * deltaTime;
            body.get().addPosition(body.get().getTotalVelocity() * deltaTime);
        }
        return;
    }

    auto earliest_it = std::min_element(collisions.begin(), collisions.end(),
    [](const auto& a, const auto& b) { return a.second < b.second; });

    const Collision& earliest_collision = earliest_it->first;


    // Move all objects to earliest_collision.collisionTime

    for (auto body: immovables) {
        body.get().impulse_velocity += body.get().acceleration * earliest_collision.collisionTime;
        body.get().addPosition(body.get().getTotalVelocity() * earliest_collision.collisionTime);
    }

    earliest_collision.objectA.intrinsic_velocity = {0, 0};
    earliest_collision.objectA.impulse_velocity = {0, 0};
    earliest_collision.objectA.friction_velocity = {0, 0};
    earliest_collision.objectA.acceleration = {0, 0};
    earliest_collision.objectB.intrinsic_velocity = {0, 0};
    earliest_collision.objectB.impulse_velocity = {0, 0};
    earliest_collision.objectB.friction_velocity = {0, 0};
    earliest_collision.objectB.acceleration = {0, 0};

    moveImmovables(deltaTime - earliest_collision.collisionTime);
}

void CollisionsHandler::moveMovables(float deltaTime) const {
    for (auto body: bodies) {
        if (body.get().type == CollidableObjectType::Movable) {
            body.get().impulse_velocity += body.get().acceleration * deltaTime;
            body.get().addPosition(body.get().getTotalVelocity() * deltaTime);
        }
    }
}

ContactsPtrHashMap CollisionsHandler::buildContacts(float deltaTime) const {
    ContactsPtrHashMap contacts;

    // Build Contacts HashMap
    for (auto bodyA: bodies) {
        for (auto bodyB: bodies) {
            // ~O(1) nested for loop, very few rectangles in every hitbox
            std::unordered_map<Collision, float, CollisionHash> body_ab_collisions;
            for (std::size_t indexA = 0; indexA < bodyA.get().getHitbox().getRects().size(); indexA++) {
                auto rectA = bodyA.get().getHitbox().getRects()[indexA];
                for (std::size_t indexB = 0; indexB < bodyB.get().getHitbox().getRects().size(); indexB++) {
                    auto rectB = bodyB.get().getHitbox().getRects()[indexB];
                    if (&bodyA.get() <= &bodyB.get()) {
                        continue;
                    }
                    auto incompleteCollision =
                        sweptCollision(
                            rectA,
                            rectB,
                            bodyA.get().getTotalVelocity(),
                            bodyB.get().getTotalVelocity(),
                            deltaTime);
                    if (incompleteCollision && !(bodyA.get().type == CollidableObjectType::Immovable && bodyB.get().type == CollidableObjectType::Immovable)) {
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

    ContactsHandler::getInstance().reset();

    for (const auto& contact: contacts | std::views::values) {
        ContactsHandler::getInstance().addContact(Contact(contact));
    }

    return contacts;
}


ContactsPtrHashMap CollisionsHandler::buildContactsFaster(float deltaTime) {
    ContactsPtrHashMap contacts;

    buildSpatialMap();

    auto pairs = spacial_map.getPairs();

    std::cout << "Bodies: " << bodies.size() << " Pairs: " << pairs.size() << std::endl;
    for (auto [bodyA, bodyB]: pairs) {
        float earliestTime = std::numeric_limits<float>::max();
        std::optional<IncompleteCollision> earliestCollision;
        std::size_t earliestIndexA = 0, earliestIndexB = 0;
        
        for (std::size_t indexA = 0; indexA < bodyA->getHitbox().getRects().size(); indexA++) {
            auto rectA = bodyA->getHitbox().getRects()[indexA];
            for (std::size_t indexB = 0; indexB < bodyB->getHitbox().getRects().size(); indexB++) {
                auto rectB = bodyB->getHitbox().getRects()[indexB];
                auto incompleteCollision =
                    sweptCollision(
                        rectA,
                        rectB,
                        bodyA->getTotalVelocity(),
                        bodyB->getTotalVelocity(),
                        deltaTime);
                if (incompleteCollision && !(bodyA->type == CollidableObjectType::Immovable && bodyB->type == CollidableObjectType::Immovable)) {
                    if (incompleteCollision->collisionTime < earliestTime) {
                        earliestTime = incompleteCollision->collisionTime;
                        earliestCollision = incompleteCollision;
                        earliestIndexA = indexA;
                        earliestIndexB = indexB;
                    }
                }
            }
        }
        
        if (earliestCollision.has_value()) {
            contacts.emplace(std::make_pair(bodyA, bodyB), 
                           Collision{(*bodyA), (*bodyB), earliestCollision.value(), earliestIndexA, earliestIndexB});
        }
    }

    ContactsHandler::getInstance().reset();
    for (const auto& contact: contacts | std::ranges::views::values) {
        ContactsHandler::getInstance().addContact(Contact(contact));
    }

    return contacts;
}


float CollisionsHandler::getPenetration(sf::FloatRect rectA, sf::FloatRect rectB, CollisionAxis axis) {
    if (axis == CollisionAxis::Up || axis == CollisionAxis::Down) {
        if (auto intersection = rectA.findIntersection(rectB)) {
            return intersection.value().size.y;
        }
    }

    if (axis == CollisionAxis::Left || axis == CollisionAxis::Right) {
        if (auto intersection = rectA.findIntersection(rectB)) {
            return intersection.value().size.x;
        }
    }

    return 0;
}

std::optional<IncompleteCollision> CollisionsHandler::sweptCollision(
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

    if (auto intersection = rectA.findIntersection(rectB)) {
        auto size = intersection.value().size;
        if (size.x >= size.y) {
            // axis = (relative_velocity.y > 0) ? CollisionAxis::Down : CollisionAxis::Up;
            axis = (rectA.position.y > rectB.position.y) ? CollisionAxis::Down : CollisionAxis::Up;
        } else {
            // axis = (relative_velocity.x > 0) ? CollisionAxis::Left : CollisionAxis::Right;
            axis = (rectA.position.x > rectB.position.x) ? CollisionAxis::Right : CollisionAxis::Left;
        }

        return IncompleteCollision{rectA, rectB, axis, deltaTime, 0};
    }

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

void CollisionsHandler::drawHitboxes(sf::RenderWindow &window, sf::Color color) const {
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

void CollisionsHandler::buildSpatialMap() {
    auto updated = getCellSize(true);

    spacial_map = SpacialHashMap{bodies.size()};

    for (const auto& body: bodies) {
        spacial_map.addObject(&body.get());
    }
}

float CollisionsHandler::getCellSize(bool update) const {
    static float cellSize = -1;
    if (cellSize == -1 || update) {
        cellSize = getNewCellSize();
    }
    return cellSize;
}


float CollisionsHandler::getNewCellSize() const {
    std::vector<float> diameters;
    diameters.reserve(bodies.size());
    for (auto body: bodies) {
        sf::Vector2f size = body.get().getHitbox().getBounds().size;
        diameters.push_back(std::max(size.x, size.y));
    }
    return *std::ranges::max_element(diameters);
    return std::accumulate(diameters.begin(), diameters.end(), 0.f) / static_cast<float>(bodies.size());
}
