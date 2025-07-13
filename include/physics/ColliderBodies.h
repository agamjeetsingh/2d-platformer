//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef COLLIDERBODIES_H
#define COLLIDERBODIES_H
#include <unordered_set>
#include "Collider.h"

struct ColliderHash {
    std::size_t operator()(const Collider& collider) const {
        return std::hash<float>{}(collider.getPosition().x) ^
               std::hash<float>{}(collider.getPosition().y) ^
               std::hash<unsigned long>{}(collider.getHitbox().size());
    }
};

class ColliderBodies {
public:
    static ColliderBodies& getInstance();

    [[nodiscard]] const std::unordered_set<Collider, ColliderHash>& getBodies() const;

    void addBody(Collider body);

    void removeBody(Collider body);

private:
    std::unordered_set<Collider, ColliderHash> bodies;
};

#endif //COLLIDERBODIES_H
