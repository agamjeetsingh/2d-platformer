//
// Created by Agamjeet Singh on 29/07/25.
//

#include "../../include/physics/GlobalIsland.h"
#include "../../include/entity/CollidableObject.h"

std::shared_ptr<Island> GlobalIsland::getInstance() {
    static std::shared_ptr<GlobalIsland> instance = nullptr;
    if (!instance) {
        // Have to keep constructor public
        instance = std::make_shared<GlobalIsland>();
        instance->leader.get().joinIslandAsLeader(instance);
    }
    return instance;
}

GlobalIsland::GlobalIsland(): Island(CollidableObject::getPointLeader()) {}
