//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../include/physics/ColliderBodies.h"

ColliderBodies& ColliderBodies::getInstance() {
    static ColliderBodies instance;
    return instance;
}

[[nodiscard]] const std::unordered_set<Collider, ColliderHash>& ColliderBodies::getBodies() const {
    return bodies;
}

void ColliderBodies::addBody(Collider body) {
    bodies.insert(body);
}

void ColliderBodies::removeBody(Collider body) {
    bodies.erase(body);
}

