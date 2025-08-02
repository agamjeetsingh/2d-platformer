//
// Created by Agamjeet Singh on 28/07/25.
//

#include "Island.h"
#include "../entity/CollidableObject.h"

Island::Island(CollidableObject& leader): leader(leader) {
    assert(leader.type == CollidableObjectType::Immovable);
    objects = { leader };
}

const CollidableObject& Island::getLeader() const {
    return leader;
}

std::vector<std::reference_wrapper<CollidableObject>> Island::getNonLeaders() const {
    std::vector<std::reference_wrapper<CollidableObject>> non_leader_objects;
    non_leader_objects.reserve(objects.size());
    for (const auto& object: objects) {
        if (object.get() == leader) {
            continue;
        }
        non_leader_objects.push_back(object);
    }

    return non_leader_objects;
}

const std::vector<std::reference_wrapper<CollidableObject>>& Island::getMembers() const {
    return objects;
}

void Island::addMember(CollidableObject& object) {
    assert(object.type != CollidableObjectType::Immovable);
    objects.emplace_back(object);
}

bool Island::removeMember(CollidableObject& object) {
    if (leader.get() == object) {
        return false;
    }

    const auto removed = std::erase_if(objects, [&](const std::reference_wrapper<CollidableObject>& ref) {
        return &ref.get() == &object;
    });
    return removed;
}

sf::Vector2f Island::getLeaderVelocity() const {
    return leader.get().velocity;
}

sf::Vector2f Island::getLeaderAcceleration() const {
    return leader.get().acceleration;
}

void Island::setLeader(CollidableObject& new_leader) {
    assert(new_leader.type == CollidableObjectType::Immovable);
    std::erase_if(objects,
                      [this](const std::reference_wrapper<CollidableObject>& ref) {
                          return &ref.get() == &leader.get();
    });
    leader = new_leader;
    objects.push_back(leader);
}
