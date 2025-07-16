//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../include/physics/ColliderBodies.h"

ColliderBodies& ColliderBodies::getInstance() {
    static ColliderBodies instance;
    return instance;
}

[[nodiscard]] const std::unordered_set<std::reference_wrapper<Collider>, ColliderRefHash, ColliderRefEqual>& ColliderBodies::getBodies() const {
    return bodies;
}

void ColliderBodies::addBody(Collider& body) {
    bodies.insert(std::ref(body));
}

void ColliderBodies::removeBody(Collider& body) {
    bodies.erase(std::ref(body));
}

